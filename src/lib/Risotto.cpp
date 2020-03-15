//
// Created by raphael on 21/06/18.
//

extern "C" {
#include <lib/vm/debug.h>
}

#include <fstream>
#include <lib/tokenizer/TokensPrinter.h>
#include <lib/tokenizer/Tokenizer.h>
#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/ASTPrinter.h>
#include <lib/parser/Parser.h>
#include <iostream>
#include <chrono>
#include <sstream>
#include "Risotto.h"

Risotto::Risotto(unsigned int flags) : flags(flags) {
    this->printfp = printf;
}

Risotto::Risotto() : Risotto(RisottoFlags::None) {}

InterpretResult Risotto::runFile(const std::string &path) {
    return runFile(path, {});
}

InterpretResult Risotto::runFile(const std::string &path, const std::vector<std::string> &args) {
    std::ifstream ifs(path);
    std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return run(str, args);
}

InterpretResult Risotto::run(const std::string &str) {
    return run(str, {});
}

InterpretResult Risotto::run(const std::string &str, const std::vector<std::string> &args) {
    return timing<InterpretResult>("Total", [this, str, args]() {
        return doRun(str, args);
    });
}

InterpretResult Risotto::doRun(const std::string &str, const std::vector<std::string> &args) {
    initPrimitives();

    auto tokens = timing<std::vector<Token *>>("Tokenizer", [str]() {
        auto tokenizer = new Tokenizer(str);
        return tokenizer->tokenize();
    });

    if (hasFlag(RisottoFlags::PrintTokens)) {
        TokensPrinter::print(tokens);
    }

    auto stmts = timing<std::vector<Stmt *>>("Parser", [tokens]() {
        auto parser = new Parser(tokens);
        return parser->program();
    });

    if (hasFlag(RisottoFlags::PrintAST)) {
        std::cout << ASTPrinter::print(stmts);
    }

    auto chunk = timing<Chunk>("Compiler", [stmts]() {
        return Compiler(stmts, &primitives).compile();
    });

    if (hasFlag(RisottoFlags::PrintDisassembled)) {
        disassembleChunk(&chunk, "chunk");
    }

    unsigned int vmFlags = VMFlags::VMNone;

    if (hasFlag(RisottoFlags::PrintTraceExecution)) {
        vmFlags |= VMFlags::TraceExecution;
    }

    if (hasFlag(RisottoFlags::PrintBenchmarkExecution)) {
        vmFlags |= VMFlags::BenchmarkExecution;
    }

    auto argsa = (ValueArray *) malloc(sizeof(ValueArray));
    initValueArray(argsa);
    registerObject((Object *) argsa);

    for (const auto &arg: args) {
        writeValueArray(argsa, s2v(arg.c_str()));
    }

    initVM(vmFlags, this->printfp, argsa);

    auto result = timing<InterpretResult>("VM", [&chunk]() {
        return interpret(&chunk, 0);
    });

    freeVM();
    freeChunk(&chunk);

    return result;
}

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
