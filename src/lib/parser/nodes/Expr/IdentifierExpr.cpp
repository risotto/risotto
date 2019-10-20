//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/utils/Utils.h>
#include "IdentifierExpr.h"
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"
#include "lib/compiler/TypeDefinition.h"

IdentifierExpr::IdentifierExpr(Token *name) : name(name) {

}

std::vector<ByteResolver *> IdentifierExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    auto returnType = getReturnType(compiler);

    if (!returnType.single()) {
        throw CompilerError("Must resolve to a single symbol");
    }

    if (auto functionTypeDef = dynamic_cast<FunctionTypeDefinition *>(returnType[0]->getTypeDefinition(compiler))) {
        auto functionEntry = functionTypeDef->entry;

        Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

        return bytes;
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find symbol " + name->lexeme);
    }

    bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, &name->position));
    bytes.push_back(new ByteResolver(response->distance, nullptr));
    bytes.push_back(new ByteResolver(response->variable->index, nullptr));

    return bytes;
}

ReturnTypes IdentifierExpr::computeReturnType(Compiler *compiler) {
    auto candidates = compiler->frame->findFunctionsCandidates(name->lexeme);

    auto candidateTypes = ReturnTypes();

    for (auto candidate : candidates) {
        auto params = std::vector<FunctionTypeReferenceParameter>();

        for (const auto& param: candidate->params) {
            params.emplace_back(param.name, param.type, param.asReference);
        }

        candidateTypes.push_back(new FunctionTypeReference(candidate));
    }

    auto response = compiler->frame->findVariable(name->lexeme);

    if (response != nullptr) {
        candidateTypes.push_back(response->variable->typeRef);
    }

    if (candidateTypes.empty()) {
        throw CompilerError("Cannot find symbol " + name->lexeme);
    }

    return candidateTypes;
}
