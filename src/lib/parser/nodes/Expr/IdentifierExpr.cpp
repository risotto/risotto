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

    if (shouldReturnFunctionReference) {
        auto functionEntry = getReturnType(compiler)[0]->asFunctionTypeEntry()->function;

        Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

        return bytes;
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
    if (shouldReturnFunctionReference) {
        auto candidates = compiler->frame->findFunctionsCandidates(getCandidatesFunctionsFor());

        auto candidateTypes = TypesEntries();

        for (auto candidate : candidates) {
            candidateTypes.push_back(candidate->typeEntry);
        }

        return candidateTypes;
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find symbol " + name->lexeme);
    }

    return response->variable->type;
}

std::string IdentifierExpr::getCandidatesFunctionsFor() {
    return name->lexeme;
}
