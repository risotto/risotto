#include <iostream>
#include <fstream>
#include <vector>
#include <lib/tokenizer/Tokenizer.h>
#include <lib/parser/Parser.h>
#include <lib/tokenizer/TokensPrinter.h>
#include <lib/parser/ASTPrinter.h>
#include <lib/compiler/Compiler.h>

extern "C" {
#include <lib/vm/vm.h>
#include <lib/vm/chunk.h>
#include <lib/vm/debug.h>
}

void printTime(const std::string &name, clock_t &start, clock_t &end) {
    printf("%s time : %.6fs\n", name.c_str(), (double) (end - start) / CLOCKS_PER_SEC);
}

int main(int argc, char *argv[]) {
    auto path = argv[1];

    std::ifstream ifs(path);
    std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    clock_t totalStart = clock();

    auto tokenizer = new Tokenizer(str);

    clock_t tokenizerStart = clock();
    auto tokens = tokenizer->tokenize();
    clock_t tokenizerEnd = clock();

    printTime("Tokenizer", tokenizerStart, tokenizerEnd);

    TokensPrinter::print(tokens);

    auto parser = new Parser(tokens);

    clock_t parserStart = clock();
    auto stmts = parser->program();
    clock_t parserEnd = clock();

    printTime("Parser", parserStart, parserEnd);

    std::cout << ASTPrinter::print(stmts);

    clock_t compilerStart = clock();
    auto chunk = Compiler(stmts).compile();
    clock_t compilerEnd = clock();

    printTime("Compiler", compilerStart, compilerEnd);

    clock_t vmStart = clock();

    initVM();

    disassembleChunk(&chunk, "chunk");
    interpret(&chunk, 0);

    freeVM();
    freeChunk(&chunk);
    clock_t vmEnd = clock();

    printTime("VM", vmStart, vmEnd);

    clock_t totalEnd = clock();
    printTime("Total", totalStart, totalEnd);

    return 0;
}
