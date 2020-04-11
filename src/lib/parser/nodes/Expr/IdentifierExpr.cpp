//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/CompilerError.h>
#include <lib/compiler/TypeDefinition.h>
#include <lib/parser/nodes/TypeDescriptor.h>
#include <lib/compiler/utils/Utils.h>
#include "IdentifierExpr.h"

IdentifierExpr::IdentifierExpr(const Token *name) : name(name) {

}

std::vector<ByteResolver *> IdentifierExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);

    if (!returnType.single()) {
        throw CompilerError("Must resolve to a single symbol", name->position);
    }

    if (dynamic_cast<FunctionTypeDefinition *>(returnType[0]->getTypeDefinition())) {
        auto candidates = compiler->frame->findFunctionsCandidates(name->lexeme);

        if (!candidates.empty()) {
            auto functionEntry = candidates[0];

            Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

            return bytes;
        }
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find symbol " + name->lexeme, name->position);
    }

    bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, name->position));
    bytes.push_back(new ByteResolver(response->distance));
    bytes.push_back(new ByteResolver(response->variable->index));

    return bytes;
}

ReturnTypes IdentifierExpr::computeReturnType(Compiler *compiler) {
    auto candidates = compiler->frame->findFunctionsCandidates(name->lexeme);

    auto candidateTypes = ReturnTypes();

    for (auto candidate : candidates) {
        candidateTypes.push_back(candidate->descriptor);
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response != nullptr) {
        candidateTypes.push_back(response->variable->typeRef);
    }

    if (candidateTypes.empty()) {
        throw CompilerError("Cannot find symbol " + name->lexeme, name->position);
    }

    return candidateTypes;
}

void IdentifierExpr::symbolize(Compiler *compiler) {
    // noop
}
