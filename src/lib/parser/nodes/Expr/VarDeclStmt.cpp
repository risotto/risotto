//
// Created by rvigee on 10/6/19.
//

#include "VarDeclStmt.h"

#include <utility>
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/Compiler.h>

VarDeclStmt::VarDeclStmt(std::vector<std::pair<Token *, TypeDescriptor *>> identifiers, Token *op, Expr *value) :
        identifiers(std::move(identifiers)), op(op), value(value) {}

std::vector<ByteResolver *> VarDeclStmt::compile(Compiler *compiler) {
    auto valueReturnType = value->getReturnType(compiler);

    if (valueReturnType.size() != identifiers.size()) {
        throw CompilerError("Declaration and initializer are not the same number", op->position);
    }

    for (int i = 0; i < identifiers.size(); ++i) {
        auto identifier = identifiers[i];
        auto type = identifier.second ? identifier.second : valueReturnType[i];

        compiler->frame->variables.add(identifier.first->lexeme, type);
    }

    return value->compile(compiler);
}

void VarDeclStmt::symbolize(Compiler *compiler) {
    value->symbolize(compiler);

    for (auto id: identifiers) {
        if (id.second != nullptr) {
            compiler->typesManager->add(id.second, compiler->frame);
        }
    }
}
