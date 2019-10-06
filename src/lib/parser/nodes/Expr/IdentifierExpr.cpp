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

    if (getReturnType(compiler)->isFunction()) {
        auto functionEntry = getReturnType(compiler)->asFunctionTypeEntry()->function;

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

TypeEntry *IdentifierExpr::computeReturnType(Compiler *compiler) {
    auto candidates = getCandidatesFunctions(compiler);

    if (!candidates.empty()) {
        if (candidates.size() > 1) {
            throw CompilerError("Multiple functions found for " + name->lexeme);
        }

        if (candidates.size() == 1) {
            return candidates[0]->typeEntry;
        }
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find symbol " + name->lexeme);
    }

    return response->variable->type;
}

std::vector<FunctionEntry *> IdentifierExpr::getCandidatesFunctions(Compiler *compiler) {
    return compiler->frame->findFunctionsCandidates(getCandidatesFunctionsFor());
}

std::string IdentifierExpr::getCandidatesFunctionsFor() {
    return name->lexeme;
}
