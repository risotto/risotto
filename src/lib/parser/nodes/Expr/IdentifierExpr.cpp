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

    if (!returnType.single()) {
        throw CompilerError("Must resolve to a single symbol");
    }

    if (returnType[0]->isFunction()) {
        auto functionEntry = returnType[0]->asFunctionTypeEntry()->function;

        Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

        return bytes;
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find variable " + name->lexeme);
    }

    bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, &name->position));
    bytes.push_back(new ByteResolver(response->distance, nullptr));
    bytes.push_back(new ByteResolver(response->variable->index, nullptr));

    return bytes;
}

TypesEntries IdentifierExpr::computeReturnType(Compiler *compiler) {
    auto candidates = compiler->frame->findFunctionsCandidates(name->lexeme);

    auto candidateTypes = TypesEntries();

    for (auto candidate : candidates) {
        candidateTypes.push_back(candidate->typeEntry);
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response != nullptr) {
        candidateTypes.push_back(response->variable->type);
    }

    if (candidateTypes.empty()) {
        throw CompilerError("Cannot find symbol " + name->lexeme);
    }

    return candidateTypes;
}
