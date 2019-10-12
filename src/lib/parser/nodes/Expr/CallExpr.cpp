//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include "lib/vm/value.h"
}

#include "CallExpr.h"
#include "IdentifierExpr.h"
#include "GetExpr.h"
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/TypeDefinition.h"
#include "lib/compiler/ReturnTypes.h"

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

    if (functionEntry == nullptr) {
        throw getFunctionNotFoundError(compiler);
    }

    auto bytes = std::vector<ByteResolver *>();

    auto arguments = getArguments(compiler);
    for (auto it = arguments.rbegin(); it != arguments.rend(); ++it) {
        auto argBytes = (*it)->compile(compiler);
        bytes.insert(bytes.end(), argBytes.begin(), argBytes.end());
    }

    Utils::loadFunctionEntryAddr(compiler, functionEntry, bytes);

    auto isNative = dynamic_cast<NativeFunctionEntry *>(functionEntry) != nullptr;

    if (isNative) {
        bytes.push_back(new ByteResolver(OpCode::OP_NATIVE_CALL, &rParen->position));
    } else {
        bytes.push_back(new ByteResolver(OpCode::OP_CALL, &rParen->position));
    }

    bytes.push_back(new ByteResolver(static_cast<int>(functionEntry->params.size()), nullptr));

    if (!isNative) {
        for (const auto &parameter: functionEntry->params) {
            bytes.push_back(new ByteResolver(parameter.asReference, nullptr));
        }
    }

    return bytes;
}

std::vector<TypeReference *> CallExpr::getArgumentsTypes(Compiler *compiler) {
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

ReturnTypes CallExpr::getCalleeEntry(Compiler *compiler) {
    try {
        return callee->getReturnType(compiler);
    } catch (CompilerError &e) {
        throw getFunctionNotFoundError(compiler);
    }
}

FunctionEntry *CallExpr::getFunctionEntry(Compiler *compiler) {
    auto argumentsTypes = getArgumentsTypes(compiler);

    auto calleeReturnTypes = getCalleeEntry(compiler);

    auto functions = std::vector<FunctionEntry *>();

    for (auto returnType : calleeReturnTypes.onlyFunctions()) {
        if (auto functionTypeRef = dynamic_cast<FunctionTypeReference *>(returnType)) {
            functions.push_back(functionTypeRef->entry->function);
        }
    }

    return Utils::findMatchingFunctions(functions, argumentsTypes);
}

FunctionNotFoundError CallExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto actualArgumentsTypes = Utils::getTypes(args, compiler);

    if (auto getExpr = dynamic_cast<GetExpr *>(callee)) {
        throw FunctionNotFoundError(
                getExpr->identifier->lexeme,
                getExpr->callee->getReturnType(compiler)[0]->toString(),
                actualArgumentsTypes,
                rParen
        );
    }

    if (auto identifierExpr = dynamic_cast<IdentifierExpr *>(callee)) {
        throw FunctionNotFoundError(
                identifierExpr->name->lexeme,
                "",
                actualArgumentsTypes,
                rParen
        );
    }

    auto argumentsTypes = getArgumentsTypes(compiler);

    throw FunctionNotFoundError("", "", argumentsTypes, rParen);
}

ReturnTypes CallExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

    if (functionEntry == nullptr) {
        throw getFunctionNotFoundError(compiler);
    }

    return functionEntry->returnTypes;
}
