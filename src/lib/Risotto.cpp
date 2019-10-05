//
// Created by raphael on 21/06/18.
//

#include <lib/parser/Parser.h>
#include <lib/tokenizer/Tokenizer.h>
#include <lib/compiler/Compiler.h>
#include <fstream>
#include <utility>
#include "Risotto.h"

InterpretResult Risotto::runFile(const std::string& path) {
    std::ifstream ifs(path);
    std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return run(str);
}

InterpretResult Risotto::run(std::string str) {
    auto tokenizer = new Tokenizer(std::move(str));
    auto tokens = tokenizer->tokenize();

    return doRun(tokenizer->tokenize());
}

InterpretResult Risotto::doRun(std::vector<Token *> tokens) {
    auto parser = new Parser(std::move(tokens));
    auto stmts = parser->program();

    auto chunk = Compiler(stmts).compile();

    initVM();

    auto result = interpret(&chunk, 0);

    freeVM();
    freeChunk(&chunk);

    return result;
}
