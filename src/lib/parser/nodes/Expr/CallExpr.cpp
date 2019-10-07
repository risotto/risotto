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

CallExpr::CallExpr(Expr *callee, Token *rParen, std::vector<Expr *> args) :
        CallExpr(callee, rParen, std::move(args), false) {

}

CallExpr::CallExpr(Expr *callee, Token *rParen, std::vector<Expr *> args, bool calleeIsValue) : callee(callee),
                                                                                                rParen(rParen),
                                                                                                args(std::move(args)) {
    if (auto withCandidates = dynamic_cast<ReturnsCandidatesFunctions *>(callee)) {
        if (!calleeIsValue) {
            withCandidates->forceReturnFunctionReference = true;
        }
    }
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
    auto arguments = getArguments(compiler);

    for (auto arg : arguments) {
        auto types = arg->getReturnType(compiler);

        if (!types.single()) {
            if (types.empty()) {
                throw CompilerError("This parameter doesnt return any value");
            } else if (types.size() > 1) {
                throw CompilerError("This parameter returns more than one value");
            }
        }
    }

    return Utils::getTypes(arguments, compiler);
}

FunctionEntry *CallExpr::getFunctionEntry(Compiler *compiler) {
    auto argumentsTypes = getArgumentsTypes(compiler);

    if (dynamic_cast<ReturnsCandidatesFunctions *>(callee) != nullptr) {
        auto returnTypes = callee->getReturnType(compiler);

        auto functions = std::vector<FunctionEntry *>();

        for (auto typeEntry : returnTypes) {
            if (typeEntry->isFunction()) {
                auto typeEntryFunction = typeEntry->asFunctionTypeEntry();

                functions.push_back(typeEntryFunction->function);
            } else {
                throw CompilerError("Returned types is not a function");
            }
        }

        auto entry = Utils::findMatchingFunctions(functions, argumentsTypes);

        if (entry == nullptr) {
            throw getFunctionNotFoundError(compiler);
        }

        return entry;
    }

    auto calleeReturnType = callee->getReturnType(compiler);
    if (calleeReturnType.single() && calleeReturnType[0]->isFunction()) {
        return calleeReturnType[0]->asFunctionTypeEntry()->function;
    }

    throw getFunctionNotFoundError(compiler);
}

FunctionNotFoundError CallExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto actualArgumentsTypes = Utils::getTypes(args, compiler);

    if (auto getExpr = dynamic_cast<GetExpr *>(callee)) {
        throw FunctionNotFoundError(
                getExpr->identifier->lexeme,
                getExpr->callee->getReturnType(compiler)[0]->name,
                actualArgumentsTypes,
                rParen
        );
    }

    if (auto withCandidates = dynamic_cast<ReturnsCandidatesFunctions *>(callee)) {
        throw FunctionNotFoundError(withCandidates->getCandidatesFunctionsFor(), "", actualArgumentsTypes, rParen);
    }

    auto argumentsTypes = getArgumentsTypes(compiler);

    throw FunctionNotFoundError("", "", argumentsTypes, rParen);
}

TypesEntries CallExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

    if (functionEntry == nullptr) {
        throw getFunctionNotFoundError(compiler);
    }

    return functionEntry->returnTypes;
}
