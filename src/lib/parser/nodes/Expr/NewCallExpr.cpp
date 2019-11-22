//
// Created by rvigee on 10/16/19.
//

#include <lib/compiler/Compiler.h>
#include <lib/compiler/utils/Utils.h>
#include "NewCallExpr.h"

NewCallExpr::NewCallExpr(Token *identifier, Token *rParen, const std::vector<Expr *> &args) : IdentifierCallExpr(identifier, rParen, args) {}

VariableEntry *NewCallExpr::getVariableEntry(Compiler *compiler) {
    return nullptr;
}

StructTypeDefinition *NewCallExpr::getStructTypeDef(Compiler *compiler) {
    auto typeDef = compiler->frame->findNamedType(identifier->lexeme);

    return dynamic_cast<StructTypeDefinition *>(typeDef->getTypeDefinition());
}

FunctionEntry *NewCallExpr::getFunctionEntry(Compiler *compiler) {
    auto structTypeDef = getStructTypeDef(compiler);

    return Utils::findMatchingFunctions(structTypeDef->constructors, getArgumentsTypes(compiler));
}

std::vector<TypeDescriptor *> NewCallExpr::getArgumentsTypes(Compiler *compiler) {
    auto types = BaseCallExpr::getArgumentsTypes(compiler);

    auto structTypeDef = getStructTypeDef(compiler);
    types.insert(types.begin(), new IdentifierTypeDescriptor(identifier->lexeme, structTypeDef));

    return types;
}

void NewCallExpr::loadArgs(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    BaseCallExpr::loadArgs(compiler, bytes);

    auto structTypeDef = getStructTypeDef(compiler);

    bytes.push_back(new ByteResolver(OP_NEW, nullptr));
    bytes.push_back(new ByteResolver(structTypeDef->fields.size(), nullptr));
}

FunctionNotFoundError NewCallExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto actualArgumentsTypes = Utils::getTypes(args, compiler);

    auto argumentsTypes = getArgumentsTypes(compiler);
    auto actualArgsTypes = std::vector<TypeDescriptor *>(argumentsTypes.begin() + 1, argumentsTypes.end());

    throw FunctionNotFoundError("new " + identifier->lexeme + "({{args}})", actualArgsTypes, rParen);
}
