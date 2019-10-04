//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include "lib/vm/value.h"
}

#include "CallExpr.h"

#include <utility>
#include <lib/compiler/Compiler.h>
#include <lib/compiler/CompilerError.h>

CallExpr::CallExpr(Token *identifier, Token *rParen, std::vector<Expr *> args): identifier(identifier), rParen(rParen), args(std::move(args)) {

}

std::vector<Expr *> CallExpr::getArguments(Compiler *compiler) {
    return args;
}

FunctionEntry *CallExpr::getFunctionEntry(Compiler *compiler) {
    return compiler->frame->findFunction(identifier->lexeme);
}

std::vector<ByteResolver *> CallExpr::compile(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

    if (functionEntry == nullptr) {
        throw CompilerError("Cannot find type for " + identifier->lexeme);
    }

    auto bytes = std::vector<ByteResolver *>();

    auto arguments = getArguments(compiler);
    for (auto it = arguments.rbegin(); it != arguments.rend(); ++it) {
        auto a = (*it)->compile(compiler);
        bytes.insert(bytes.end(), a.begin(), a.end());
    }

    if (auto nativeEntry = dynamic_cast<NativeFunctionEntry *>(functionEntry)) {
        auto constAddr = compiler->registerConst(p2v((void *) nativeEntry->fun));

        bytes.push_back(new ByteResolver(OpCode::OP_NATIVE_CALL, nullptr));
        bytes.push_back(new ByteResolver([constAddr](Compiler *p) { return constAddr; }, nullptr));
        bytes.push_back(new ByteResolver(static_cast<int>(arguments.size()), nullptr));

        return bytes;
    }

    bytes.push_back(new ByteResolver(OpCode::OP_CALL, &identifier->position));
    bytes.push_back(new ByteResolver([functionEntry](Compiler *c) { return c->getAddr(functionEntry->firstByte); }, nullptr));
    bytes.push_back(new ByteResolver(static_cast<int>(arguments.size()), nullptr));

    for (const auto &parameter: functionEntry->params) {
        bytes.push_back(new ByteResolver(false, nullptr)); // TODO: remove
    }

    return bytes;
}

TypeEntry *CallExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

    if (functionEntry == nullptr) {
        throw CompilerError("Cannot find type for " + identifier->lexeme);
    }

    return functionEntry->returnType;
}
