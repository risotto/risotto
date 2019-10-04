//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include "VariableExpr.h"

VariableExpr::VariableExpr(Token *name) : name(name) {

}

std::vector<ByteResolver *> VariableExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find variable " + name->lexeme);
    }

    bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, nullptr));
    bytes.push_back(new ByteResolver(response->distance, nullptr));
    bytes.push_back(new ByteResolver(response->variable->index, nullptr));

    return bytes;
}

TypeEntry *VariableExpr::computeReturnType(Compiler *compiler) {
    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find variable " + name->lexeme);
    }

    return response->variable->type;
}
