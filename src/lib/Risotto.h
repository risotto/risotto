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

class Risotto {
public:
    InterpretResult runFile(const std::string &filename);

    InterpretResult run(std::string program);

private:
    InterpretResult doRun(std::vector<Token *> tokens);
};


#endif //RISOTTOPROJECT_RISOTTO_H
