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

    if (shouldReturnFunctionReference) {
        auto returnType = getReturnType(compiler);

        if (returnType[0]->isFunction()) {
            Utils::loadFunctionEntryAddr(compiler, returnType[0]->asFunctionTypeEntry()->function, bytes);

            return bytes;
        }
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

    if (shouldReturnFunctionReference) {
        auto candidates = calleeType[0]->functions.findCandidates(getCandidatesFunctionsFor());

        auto candidateTypes = TypesEntries();

        for (auto candidate : candidates) {
            candidateTypes.push_back(candidate->typeEntry);
        }

        return candidateTypes;
    }

    throw CompilerError("Not implemented");
}
