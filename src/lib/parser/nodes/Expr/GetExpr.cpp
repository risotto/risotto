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

    if (returnType[0]->isFunction()) {
        Utils::loadFunctionEntryAddr(compiler, returnType[0]->asFunctionTypeEntry()->function, bytes);

        return bytes;
    }

    throw CompilerError("Not implemented");

    return bytes;
}

std::vector<FunctionEntry *> GetExpr::getCandidatesFunctions(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    return calleeType[0]->functions.findCandidates(getCandidatesFunctionsFor());
}

std::string GetExpr::getCandidatesFunctionsFor() {
    return identifier->lexeme;
}

TypesEntries GetExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    auto candidates = getCandidatesFunctions(compiler);

    if (candidates.empty()) {
        throw FunctionNotFoundError(identifier, calleeType[0]->name);
    }

    if (candidates.size() > 1) {
        throw CompilerError("Multiple functions match");
    }

    return candidates[0]->typeEntry;
}
