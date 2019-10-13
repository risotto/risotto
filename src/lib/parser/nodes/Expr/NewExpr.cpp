//
// Created by rvigee on 10/13/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/utils/Utils.h>
#include "NewExpr.h"
#include "lib/parser/nodes/TypeDescriptor.h"
#include "lib/compiler/TypeDefinition.h"

NewExpr::NewExpr(TypeDescriptor *identifier) : identifier(identifier) {}

std::vector<ByteResolver *> NewExpr::compile(Compiler *compiler) {
    auto structType = getStructDef(compiler);

    auto bytes = std::vector<ByteResolver *>();

    bytes.push_back(new ByteResolver(OP_NEW, nullptr));
    bytes.push_back(new ByteResolver(structType->fields.size(), nullptr));

    return bytes;
}

ReturnTypes NewExpr::computeReturnType(Compiler *compiler) {
    auto structType = getStructDef(compiler);

    ReturnTypes returnTypes;

    for (const auto &constructor : structType->constructors) {
        returnTypes.push_back(new FunctionTypeReference(constructor->typeDefinition));
    }

    return returnTypes;
}

StructTypeDefinition *NewExpr::getStructDef(Compiler *compiler) {
    auto ref = identifier->toTypeReference(compiler);

    if (auto concrete = dynamic_cast<ConcreteTypeReference *>(ref)) {
        auto structType = dynamic_cast<StructTypeDefinition *>(concrete->entry);

        if (structType == nullptr) {
            throw CompilerError(identifier->toString() + " is not a struct");
        }

        return structType;
    }

    throw CompilerError("Identifier type must be concrete");
}

