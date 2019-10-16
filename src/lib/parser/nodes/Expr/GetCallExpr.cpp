//
// Created by rvigee on 10/16/19.
//

#include <lib/compiler/utils/Utils.h>
#include "GetCallExpr.h"
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeDefinition.h"

GetCallExpr::GetCallExpr(Expr *callee, Token *op, Token *identifier, Token *rParen, const std::vector<Expr *> &args)
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
    auto args = getArgumentsTypes(compiler);

    return FunctionNotFoundError(op->lexeme, leftReturnType[0]->toString(), args, identifier);
}

VariableEntry *GetCallExpr::getVariableEntry(Compiler *compiler) {
    return nullptr;
}

FunctionEntry *GetCallExpr::getFunctionEntry(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Return type has to be single", identifier->position);
    }

    auto returnTypes = ReturnTypes();

    if (auto receiver = dynamic_cast<ReceiverTypeReference *>(calleeType[0])) {
        auto functionsCandidates = receiver->findFunctionsCandidates(compiler->frame, identifier->lexeme);

        return Utils::findMatchingFunctions(functionsCandidates, getArgumentsTypes(compiler));
    }

    return nullptr;
}

void GetCallExpr::loadVariableEntryAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    throw CompilerError("Unimplemented", identifier->position);
}
