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

GetExpr::GetExpr(Expr *callee, Token *identifier) : callee(callee), identifier(identifier) {

}

std::vector<ByteResolver *> GetExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);
    auto functionsReturnType = returnType.onlyFunctions();

    if (!functionsReturnType.empty()) {
        if (returnType[0]->isFunction()) {
            Utils::loadFunctionEntryAddr(compiler, returnType[0]->asFunctionTypeEntry()->function, bytes);

            return bytes;
        }
    }

    if (forceReturnFunctionReference) {
        throw CompilerError("A single function must be returned", identifier->position);
    }

    throw CompilerError("Not implemented");

    return bytes;
}

std::string GetExpr::getCandidatesFunctionsFor() {
    return identifier->lexeme;
}

TypesEntries GetExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Return type has to be single", identifier->position);
    }

    auto candidates = calleeType[0]->functions.findCandidates(getCandidatesFunctionsFor());

    auto candidateTypes = TypesEntries();

    for (auto candidate : candidates) {
        candidateTypes.push_back(candidate->typeEntry);
    }

    if (forceReturnFunctionReference) {
        return candidateTypes;
    }

    throw CompilerError("Not implemented");

    return candidateTypes;
}
