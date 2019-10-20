//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_CALLEXPR_H
#define RISOTTOV2_CALLEXPR_H


#include <vector>
#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>
#include <lib/compiler/CompilerError.h>
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"

class BaseCallExpr : public Expr {
public:
    Token *rParen;
    std::vector<Expr *> args;

    BaseCallExpr(Token *rParen, std::vector<Expr *> args);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    virtual std::vector<TypeReference *> getArgumentsTypes(Compiler *compiler);

    virtual std::vector<Expr *> getArguments(Compiler *compiler);

    virtual FunctionNotFoundError getFunctionNotFoundError(Compiler *compiler);

    virtual bool isArgumentReference(Compiler *compiler, int i) = 0;

    virtual void loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) = 0;
    virtual void loadArgs(Compiler *compiler, std::vector<ByteResolver *> &bytes);

    virtual bool needAddrResolution(Compiler *compiler);
};

class CallExpr : public BaseCallExpr {
public:
    Expr *callee;

    CallExpr(Expr *callee, Token *rParen, const std::vector<Expr *>& args);

    bool isArgumentReference(Compiler *compiler, int i) override;

    void loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_CALLEXPR_H
