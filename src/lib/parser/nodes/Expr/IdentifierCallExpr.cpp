//
// Created by rvigee on 10/16/19.
//

#include <lib/compiler/utils/Utils.h>
#include "IdentifierCallExpr.h"
#include "lib/compiler/Compiler.h"

IdentifierCallExpr::IdentifierCallExpr(const Token *identifier, const Token *rParen, const std::vector<Expr *> &args)
        : MixedCallExpr(rParen, args), identifier(identifier) {}

VariableEntry *IdentifierCallExpr::getVariableEntry(Compiler *compiler) {
    return compiler->frame->variables.find(identifier->lexeme);
}

FunctionEntry *IdentifierCallExpr::getFunctionEntry(Compiler *compiler) {
    auto types = getArgumentsTypes(compiler);

    return compiler->frame->findFunction(identifier->lexeme, types);
}

void IdentifierCallExpr::loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    auto response = compiler->frame->findVariable(identifier->lexeme);

    if (response == nullptr) {
        throw CompilerError("Cannot find variable " + identifier->lexeme);
    }

    bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, identifier->position));
    bytes.push_back(new ByteResolver(response->distance));
    bytes.push_back(new ByteResolver(response->variable->index));
}

FunctionNotFoundError IdentifierCallExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto actualArgumentsTypes = Utils::getTypes(args, compiler);

    auto argumentsTypes = getArgumentsTypes(compiler);

    return FunctionNotFoundError(identifier->lexeme + "({{args}})", argumentsTypes, rParen);
}
