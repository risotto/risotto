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
    auto tokens = timing<std::vector<Token>>("Tokenizer", [str]() {
        return Tokenizer::Tokenize(str);
    });

    if (hasFlag(RisottoFlags::PrintTokens)) {
        this->printfp("%s", TokensPrinter::print(tokens).c_str());
    }

    auto stmts = timing<std::vector<Stmt *>>("Parser", [&tokens]() {
        return Parser::Parse(&tokens);
    });

    if (hasFlag(RisottoFlags::PrintAST)) {
        this->printfp("%s", ASTPrinter::print(stmts).c_str());
    }

    auto chunk = timing<Chunk>("Compiler", [stmts]() {
        return Compiler(stmts, &primitives).compile();
    });

    unsigned int vmFlags = VMFlags::VMNone;

#ifdef DEBUG_TRACE_EXECUTION
    if (hasFlag(RisottoFlags::PrintTraceExecution)) {
        vmFlags |= VMFlags::TraceExecution;
    }
#endif

#ifdef BENCHMARK_TIMINGS
    if (hasFlag(RisottoFlags::PrintBenchmarkExecution)) {
        vmFlags |= VMFlags::BenchmarkExecution;
    }
#endif

    auto argsa = ValueArray{};
    vec_init(&argsa.vec);

    for (const auto &arg: args) {
        vec_push(&argsa.vec, s2v(arg.c_str()));
    }

    initVM(vmFlags, this->printfp, &argsa);

    if (hasFlag(RisottoFlags::PrintDisassembled)) {
        disassembleChunk(&chunk, "chunk");
    }

    auto result = timing<InterpretResult>("VM", [&chunk]() {
        return interpret(&chunk, 0);
    });

    freeVM();
    freeChunk(&chunk);

    return result;
}

bool Risotto::hasFlag(RisottoFlags flag) const {
    return (flags & flag) == flag;
}

template<class Rep, class Period>
std::string nsToStr(std::chrono::duration<Rep, Period> duration) {
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
