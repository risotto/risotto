//
// Created by raphael on 21/06/18.
//

#include <lib/parser/Parser.h>
#include <lib/tokenizer/Tokenizer.h>
#include <lib/compiler/Compiler.h>
#include <fstream>
#include <iostream>
#include "Risotto.h"
#include <iomanip>
#include <chrono>
#include <sstream>
#include <lib/tokenizer/TokensPrinter.h>
#include <lib/parser/ASTPrinter.h>

extern "C" {
#include <lib/vm/debug.h>
}

InterpretResult Risotto::runFile(const std::string &path) {
    std::ifstream ifs(path);
    std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return run(str);
}

InterpretResult Risotto::run(const std::string &str) {
    return timing<InterpretResult>("Total", [this, str]() {
        auto tokens = timing<std::vector<Token *>>("Tokenizer", [str]() {
            auto tokenizer = new Tokenizer(str);
            return tokenizer->tokenize();
        });

        if (hasFlag(RisottoFlags::PrintTokens)) {
            TokensPrinter::print(tokens);
        }

        return doRun(tokens);
    });
}

InterpretResult Risotto::doRun(const std::vector<Token *> &tokens) {
    auto stmts = timing<std::vector<Stmt *>>("Parser", [tokens]() {
        auto parser = new Parser(tokens);
        return parser->program();
    });

    if (hasFlag(RisottoFlags::PrintAST)) {
        std::cout << ASTPrinter::print(stmts);
    }

    auto chunk = timing<Chunk>("Compiler", [stmts]() {
        return Compiler(stmts).compile();
    });

    if (hasFlag(RisottoFlags::PrintDisassembled)) {
        disassembleChunk(&chunk, "chunk");
    }

    initVM();

    auto result = timing<InterpretResult>("VM", [&chunk]() {
        return interpret(&chunk, 0);
    });

    freeVM();
    freeChunk(&chunk);

    return result;
}

Risotto::Risotto(unsigned int flags) : flags(flags) {}

bool Risotto::hasFlag(RisottoFlags flag) {
    return (flags & flag) == flag;
}

template<class _Rep, class _Period>
std::string nsToStr(std::chrono::duration<_Rep, _Period> duration) {
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    duration -= seconds;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
    duration -= milliseconds;
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration);
    duration -= microseconds;
    auto nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(duration);

    typedef struct {
        std::string s;
        std::function<long()> f;

    } Unit;

    auto units = std::vector<Unit>{
            {
                    .s =  "h",
                    .f = [hours]() { return hours.count(); },
            },
            {
                    .s =  "m",
                    .f = [minutes]() { return minutes.count(); },
            },
            {
                    .s =  "s",
                    .f = [seconds]() { return seconds.count(); },
            },
            {
                    .s =  "ms",
                    .f = [milliseconds]() { return milliseconds.count(); },
            },
            {
                    .s =  "us",
                    .f = [microseconds]() { return microseconds.count(); },
            },
            {
                    .s =  "ns",
                    .f = [nanoseconds]() { return nanoseconds.count(); },
            },
    };

    std::stringstream ss;

    for (const auto &unit:units) {
        auto v = unit.f();
        if (v > 0) {
            ss << v << unit.s << " ";
        }
    }

    return ss.str();
}

template<typename T>
T Risotto::timing(const std::string &name, std::function<T()> f) {
    if (hasFlag(PrintTimings)) {
        auto start = std::chrono::high_resolution_clock::now();
        auto r = f();
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<unsigned long, std::nano> elapsed = end - start;

        std::cout.precision(7);
        std::cout << name << " time: " << nsToStr(elapsed) << std::endl;

        return r;
    } else {
        return f();
    }
}
