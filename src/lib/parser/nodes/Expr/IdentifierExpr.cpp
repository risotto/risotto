//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/utils/Utils.h>
#include "IdentifierExpr.h"

IdentifierExpr::IdentifierExpr(Token *name) : name(name) {

}

std::vector<ByteResolver *> IdentifierExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);
    auto functionsReturnType = returnType.onlyFunctions();

    if (!functionsReturnType.empty()) {
        if (!functionsReturnType.single()) {
            throw CompilerError("Return type has to be single", name->position);
        }

        auto functionEntry = functionsReturnType[0]->asFunctionTypeEntry()->function;

        Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

        return bytes;
    }

    if (forceReturnFunctionReference) {
        throw CompilerError("A single function must be returned", name->position);
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find variable " + name->lexeme);
    }

    bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, nullptr));
    bytes.push_back(new ByteResolver(response->distance, nullptr));
    bytes.push_back(new ByteResolver(response->variable->index, nullptr));

    return bytes;
}

TypesEntries IdentifierExpr::computeReturnType(Compiler *compiler) {
    auto candidates = compiler->frame->findFunctionsCandidates(getCandidatesFunctionsFor());

    auto candidateTypes = TypesEntries();

    for (auto candidate : candidates) {
        candidateTypes.push_back(candidate->typeEntry);
    }

    if (forceReturnFunctionReference) {
        return candidateTypes;
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find symbol " + name->lexeme);
    }

    candidateTypes.push_back(response->variable->type);

    return candidateTypes;
}

std::string IdentifierExpr::getCandidatesFunctionsFor() {
    return name->lexeme;
}
