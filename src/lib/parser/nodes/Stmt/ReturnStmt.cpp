//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include <utility>
#include "ReturnStmt.h"

ReturnStmt::ReturnStmt(Token *keyword, std::vector<Expr *> values) : keyword(keyword), values(std::move(values)) {

}

std::vector<ByteResolver *> ReturnStmt::compile(Compiler *compiler) {
    auto distance = compiler->frame->findFrame(FUNCTION);

    if (distance < 0) {
        throw CompilerError("Cannot return from outside a function");
    }

    auto bytes = std::vector<ByteResolver *>();

    for (auto value : values) {
        auto valueBytes = value->compile(compiler);
        bytes.insert(bytes.end(), valueBytes.begin(), valueBytes.end());
    }

    bytes.push_back(new ByteResolver(OpCode::OP_RETURN, keyword->position));

    return bytes;
}

void ReturnStmt::symbolize(Compiler *compiler) {
    for (auto value: values) {
        value->symbolize(compiler);
    }
}
