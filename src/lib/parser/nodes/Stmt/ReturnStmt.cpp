//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include "ReturnStmt.h"

ReturnStmt::ReturnStmt(Token *keyword, Expr *value) : keyword(keyword), value(value) {

}

std::vector<ByteResolver *> ReturnStmt::compile(Compiler *compiler) {
    auto distance = compiler->frame->findFrame(FUNCTION);

    if (distance < 0) {
        throw CompilerError("Cannot  return from outside a function");
    }

    auto bytes = std::vector<ByteResolver *>();

    if (value) {
        auto valueBytes = value->compile(compiler);
        bytes.insert(bytes.end(), valueBytes.begin(), valueBytes.end());
    }

    bytes.push_back(new ByteResolver(OpCode::OP_RETURN, &keyword->position));
    bytes.push_back(new ByteResolver(distance, nullptr));
    bytes.push_back(new ByteResolver(value ? 1 : 0, nullptr));

    return bytes;
}
