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

int main(int argc, char *argv[]) {
    auto path = argv[1];

    std::ifstream ifs(path, std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    auto str = std::string(bytes.data(), fileSize);

    auto tokenizer = new Tokenizer(str);

    auto tokens = tokenizer->tokenize();

    TokensPrinter::print(tokens);

    auto parser = new Parser(tokens);

    auto stmts = parser->program();

    std::cout << ASTPrinter::print(stmts);

    auto chunk = Compiler(stmts).compile();

    initVM();

    disassembleChunk(&chunk, "chunk");
    interpret(&chunk, 0);

    freeVM();
    freeChunk(&chunk);

    return 0;
}
