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
#include "lib/parser/nodes/TypeDescriptor.h"
#include "lib/compiler/ReturnTypes.h"
#include "NewExpr.h"

#include <utility>
#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>
#include <lib/compiler/utils/Utils.h>

BaseCallExpr::BaseCallExpr(Token *rParen, std::vector<Expr *> args) : rParen(rParen), args(std::move(args)) {}

std::vector<ByteResolver *> BaseCallExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    loadArgs(compiler, bytes);

    loadCallAddr(compiler, bytes);

    auto argsTypes = getArgumentsTypes(compiler);
    auto argc = argsTypes.size();

    bytes.push_back(new ByteResolver(OpCode::OP_CALL, &rParen->position));
    bytes.push_back(new ByteResolver(static_cast<int>(argc), nullptr));

    for (int i = 0; i < argc; ++i) {
        bytes.push_back(new ByteResolver(isArgumentReference(compiler, i), nullptr));
    }

    return bytes;
}

void BaseCallExpr::loadArgs(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    auto arguments = getArguments(compiler);

    for (auto it = arguments.rbegin(); it != arguments.rend(); ++it) {
        auto argBytes = (*it)->compile(compiler);
        bytes.insert(bytes.end(), argBytes.begin(), argBytes.end());
    }
}

std::vector<TypeReference *> BaseCallExpr::getArgumentsTypes(Compiler *compiler) {
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

FunctionNotFoundError BaseCallExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto actualArgumentsTypes = Utils::getTypes(args, compiler);

    auto argumentsTypes = getArgumentsTypes(compiler);

    throw FunctionNotFoundError("", "", argumentsTypes, rParen);
}

std::vector<Expr *> BaseCallExpr::getArguments(Compiler *compiler) {
    return args;
}

CallExpr::CallExpr(Expr *callee, Token *rParen, const std::vector<Expr *> &args) :
        BaseCallExpr(rParen, args), callee(callee) {
}

ReturnTypes CallExpr::computeReturnType(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Callee type must be single");
    }

    if (auto functionType = dynamic_cast<FunctionTypeReference *>(calleeType[0])) {
        return functionType->returnTypes;
    }

    throw CompilerError("Return type must be a function");
}

bool CallExpr::isArgumentReference(Compiler *compiler, int i) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Callee type must be single");
    }

    if (auto functionType = dynamic_cast<FunctionTypeReference *>(calleeType[0])) {
        return functionType->params[i].asReference;
    }

    throw CompilerError("Return type must be a function");
}

void CallExpr::loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    auto calleeBytes = callee->compile(compiler);
    bytes.insert(bytes.end(), calleeBytes.begin(), calleeBytes.end());
}
