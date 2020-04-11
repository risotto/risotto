//
// Created by rvigee on 10/16/19.
//

#include <cassert>
#include "GetCallExpr.h"
#include <lib/compiler/utils/Utils.h>

GetCallExpr::GetCallExpr(Expr *callee, PToken op, PToken identifier, PToken rParen, const std::vector<Expr *> &args)
        : MixedCallExpr(rParen, args), callee(callee), op(op), identifier(identifier) {}

std::vector<Expr *> GetCallExpr::getArguments(Compiler *compiler) {
    auto args = std::vector<Expr *>();
    args.push_back(callee);

    auto baseArgs = BaseCallExpr::getArguments(compiler);
    args.insert(args.end(), baseArgs.begin(), baseArgs.end());

    return args;
}

FunctionNotFoundError GetCallExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto leftReturnType = callee->getReturnType(compiler);
    auto argsTypes = getArgumentsTypes(compiler);
    auto actualArgsTypes = std::vector<TypeDescriptor *>(argsTypes.begin() + 1, argsTypes.end());

    return FunctionNotFoundError(leftReturnType[0]->toString() + "." + identifier->lexeme + "({{args}})", actualArgsTypes, identifier);
}

VariableEntry *GetCallExpr::getVariableEntry(Compiler *compiler) {
    return nullptr;
}

FunctionEntry *GetCallExpr::getFunctionEntry(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Return type has to be single", identifier->position);
    }

    auto typeDef = calleeType[0]->getTypeDefinition();
    assert(typeDef != nullptr);
    auto functionsCandidates = typeDef->functions.findCandidates(identifier->lexeme);

    return Utils::findMatchingFunctions(functionsCandidates, getArgumentsTypes(compiler));
}

void GetCallExpr::loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    throw CompilerError("Unimplemented", identifier->position);
}

void GetCallExpr::symbolize(Compiler *compiler) {
    callee->symbolize(compiler);
    MixedCallExpr::symbolize(compiler);
}
