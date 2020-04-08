//
// Created by raphael on 21/06/18.
//

#ifndef RISOTTOPROJECT_RISOTTO_H
#define RISOTTOPROJECT_RISOTTO_H

extern "C" {
#include <lib/vm/vm.h>
}

#include <string>
#include <vector>
#include <lib/tokenizer/Token.h>
#include <functional>

enum RisottoFlags {
    None                      = 0,
    PrintTimings              = 1 << 0,
    PrintTokens               = 1 << 1,
    PrintAST                  = 1 << 2,
    PrintDisassembled         = 1 << 3,
#ifdef DEBUG_TRACE_EXECUTION
    PrintTraceExecution       = 1 << 4,
#endif
#ifdef BENCHMARK_TIMINGS
    PrintBenchmarkExecution   = 1 << 5,
#endif
};

class Risotto {
public:
    const unsigned int flags;

    RisottoPrintf printfp;

    Risotto();

    explicit Risotto(unsigned int flags);

    InterpretResult runFile(const std::string &filename);

    InterpretResult runFile(const std::string &path, const std::vector<std::string>& args);

    InterpretResult run(const std::string &program);

    InterpretResult run(const std::string &program, const std::vector<std::string>& args);

    bool hasFlag(RisottoFlags flag);

    template<typename T>
    T timing(const std::string &name, std::function<T()> f);


private:
    InterpretResult doRun(const std::string &str, const std::vector<std::string>& args);
};


#endif //RISOTTOPROJECT_RISOTTO_H
