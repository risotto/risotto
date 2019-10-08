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

GetExpr::GetExpr(Expr *callee, Token *identifier) : callee(callee), identifier(identifier) {

}

std::vector<ByteResolver *> GetExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);

    if (!returnType.single()) {
        throw CompilerError("Must resolve to a single symbol");
    }

    if (returnType[0].isFunction()) {
        Utils::loadFunctionEntryAddr(compiler, returnType[0].entry->asFunctionTypeEntry()->function, bytes);

        return bytes;
    }

    throw CompilerError("Not implemented");

    return bytes;
}

TypeReferences GetExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Return type has to be single", identifier->position);
    }

    auto candidates = calleeType[0].entry->functions.findCandidates(identifier->lexeme);

    auto candidateTypes = TypeReferences();

    for (auto candidate : candidates) {
        candidateTypes.push_back(TypeReference(candidate->typeEntry, false));
    }

    return candidateTypes;
}
