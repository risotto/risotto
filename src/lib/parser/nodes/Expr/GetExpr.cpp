//
// Created by rvigee on 10/6/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include "GetExpr.h"
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/utils/Utils.h>
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"

GetExpr::GetExpr(Expr *callee, Token *identifier) : callee(callee), identifier(identifier) {

}

std::vector<ByteResolver *> GetExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);

    if (!returnType.single()) {
        throw CompilerError("Must resolve to a single symbol");
    }

    if (auto functionTypeRef = dynamic_cast<FunctionTypeReference *>(returnType[0])) {
        Utils::loadFunctionEntryAddr(compiler, functionTypeRef->entry->function, bytes);

        return bytes;
    }

    throw CompilerError("Not implemented");

    return bytes;
}

ReturnTypes GetExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Return type has to be single", identifier->position);
    }

    auto returnTypes = ReturnTypes();

    if (auto namedTypeRef = dynamic_cast<NamedTypeReference *>(calleeType[0])) {
        auto functionsCandidates = namedTypeRef->entry->functions.findCandidates(identifier->lexeme);

        for (auto candidate : functionsCandidates) {
            returnTypes.push_back(new FunctionTypeReference(candidate->typeEntry));
        }
    }

    // TODO: struct fields

    return returnTypes;
}
