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

    if (returnType->isFunction()) {
        Utils::loadFunctionEntryAddr(compiler, returnType->asFunctionTypeEntry()->function, bytes);

        return bytes;
    }

    throw CompilerError("Not implemented");

    return bytes;
}

std::vector<FunctionEntry *> GetExpr::getCandidatesFunctions(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    return calleeType->functions.findCandidates(getCandidatesFunctionsFor());
}

std::string GetExpr::getCandidatesFunctionsFor() {
    return identifier->lexeme;
}

TypeEntry *GetExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    auto candidates = getCandidatesFunctions(compiler);

    if (candidates.empty()) {
        throw FunctionNotFoundError(identifier, calleeType->name);
    }

    if (candidates.size() > 1) {
        throw CompilerError("Multiple functions match");
    }

    return candidates[0]->typeEntry;
}
