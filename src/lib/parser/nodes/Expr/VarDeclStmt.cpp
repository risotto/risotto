//
// Created by rvigee on 10/6/19.
//

#include "VarDeclStmt.h"

#include <utility>
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/Compiler.h>
#include "lib/parser/nodes/TypeDescriptor.h"

VarDeclStmt::VarDeclStmt(std::vector<std::pair<PToken , TypeDescriptor *>>  identifiers, PToken op, Expr *value) :
        identifiers(std::move(identifiers)), op(op), value(value) {}

std::vector<ByteResolver *> VarDeclStmt::compile(Compiler *compiler) {
    auto valueReturnType = value->getReturnType(compiler);

    if (valueReturnType.size() != identifiers.size()) {
        throw CompilerError("Declaration and initializer are not the same number", op->position);
    }

    for (auto i = 0u; i < identifiers.size(); ++i) {
        auto identifier = identifiers[0];
        auto returnType = valueReturnType[0];

        if (identifier.second != nullptr) {
            if (!identifier.second->canReceiveType(returnType)) {
                throw CompilerError("Invalid types", op->position);
            }
        }
    }

    for (auto i = 0u; i < identifiers.size(); ++i) {
        auto identifier = identifiers[i];
        auto type = identifier.second ? identifier.second : valueReturnType[i];

        compiler->frame->variables.add(identifier.first->lexeme, type);
    }

    auto bytes = value->compile(compiler);

    for (auto i = 0u; i < identifiers.size(); ++i) {
        bytes.push_back(new ByteResolver(OP_COPY));
    }

    return bytes;
}

void VarDeclStmt::symbolize(Compiler *compiler) {
    value->symbolize(compiler);

    for (auto id: identifiers) {
        if (id.second != nullptr) {
            compiler->typesManager->createLinkUnits(id.second, compiler->frame);
        }
    }
}
