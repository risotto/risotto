//
// Created by rvigee on 10/6/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include "GetExpr.h"
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/utils/Utils.h>
#include <lib/parser/nodes/TypeDescriptor.h>
#include "lib/compiler/Compiler.h"
#include "lib/compiler/ReturnTypes.h"
#include "lib/compiler/TypeDefinition.h"

StructTypeDefinition *getStructTypeDefinition(TypeDescriptor *typeRef) {
    return dynamic_cast<StructTypeDefinition *>(typeRef->getTypeDefinition());
}

GetExpr::GetExpr(Expr *callee, const Token *identifier) : callee(callee), identifier(identifier) {

}

std::vector<ByteResolver *> GetExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);

    if (!returnType.single()) {
        throw CompilerError("Must resolve to a single symbol", identifier->position);
    }

    auto calleeReturnType = callee->getReturnType(compiler);

    if (auto structDef = getStructTypeDefinition(calleeReturnType[0])) {
        auto typeDef = callee->getReturnType(compiler);

        auto calleeBytes = callee->compile(compiler);
        bytes.insert(bytes.begin(), calleeBytes.begin(), calleeBytes.end());

        bytes.push_back(new ByteResolver(OP_LOAD_INSTANCE, identifier->position));
        bytes.push_back(new ByteResolver(structDef->getFieldIndex(structDef->fields.find(identifier->lexeme))));

        return bytes;
    }

    throw CompilerError("Unhandled type");
}

ReturnTypes GetExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Callee must return a single value", identifier->position);
    }

    auto returnTypes = ReturnTypes();

    if (auto structDef = getStructTypeDefinition(calleeType[0])) {
        auto field = structDef->fields.find(identifier->lexeme);

        if (field != nullptr) {
            returnTypes.push_back(field->typeRef);
        }
    }

    return returnTypes;
}

void GetExpr::symbolize(Compiler *compiler) {
    callee->symbolize(compiler);
}
