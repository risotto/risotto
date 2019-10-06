//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include "lib/vm/value.h"
}

#include "CallExpr.h"
#include "IdentifierExpr.h"
#include "GetExpr.h"

#include <utility>
#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/utils/Utils.h>

CallExpr::CallExpr(Expr *callee, Token *rParen, std::vector<Expr *> args) : callee(callee), rParen(rParen),
                                                                            args(std::move(args)) {

}

std::vector<Expr *> CallExpr::getArguments(Compiler *compiler) {
    if (auto getExpr = dynamic_cast<GetExpr *>(callee)) {
        auto allArgs = std::vector<Expr *>(args);
        allArgs.insert(allArgs.begin(), getExpr->callee);

        return allArgs;
    }

    return args;
}

std::vector<ByteResolver *> CallExpr::compile(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

    auto bytes = std::vector<ByteResolver *>();

    auto arguments = getArguments(compiler);
    for (auto it = arguments.rbegin(); it != arguments.rend(); ++it) {
        auto argBytes = (*it)->compile(compiler);
        bytes.insert(bytes.end(), argBytes.begin(), argBytes.end());
    }

    Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

    if (dynamic_cast<NativeFunctionEntry *>(functionEntry) != nullptr) {
        bytes.push_back(new ByteResolver(OpCode::OP_NATIVE_CALL, &rParen->position));
    } else {
        bytes.push_back(new ByteResolver(OpCode::OP_CALL, &rParen->position));
    }

    bytes.push_back(new ByteResolver(static_cast<int>(functionEntry->params.size()), nullptr));

    return bytes;
}

std::vector<TypeEntry *> CallExpr::getArgumentsTypes(Compiler *compiler) {
    return Utils::getTypes(getArguments(compiler), compiler);
}

FunctionEntry *CallExpr::getFunctionEntry(Compiler *compiler) {
    auto argumentsTypes = getArgumentsTypes(compiler);

    if (auto withCandidates = dynamic_cast<ReturnsCandidatesFunctions *>(callee)) {
        auto functions = withCandidates->getCandidatesFunctions(compiler);

        auto entry = Utils::findMatchingFunctions(functions, argumentsTypes);

        if (entry == nullptr) {
            auto actualArgumentsTypes = Utils::getTypes(args, compiler);

            if (auto getExpr = dynamic_cast<GetExpr *>(callee)) {
                throw FunctionNotFoundError(getExpr->identifier->lexeme, getExpr->callee->getReturnType(compiler)->name, actualArgumentsTypes, rParen);
            }

            throw FunctionNotFoundError(withCandidates->getCandidatesFunctionsFor(), "", actualArgumentsTypes, rParen);
        }

        return entry;
    }

    auto calleeReturnType = callee->getReturnType(compiler);
    if (calleeReturnType->isFunction()) {
        return calleeReturnType->asFunctionTypeEntry()->function;
    }

    throw FunctionNotFoundError("", "", argumentsTypes, rParen);
}

TypeEntry *CallExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

    return functionEntry->returnType;
}
