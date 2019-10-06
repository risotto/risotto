//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_CALLEXPR_H
#define RISOTTOV2_CALLEXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>
#include <vector>
#include <lib/compiler/CompilerError.h>

class CallExpr: public Expr {
public:
    Expr *callee;
    Token *rParen;
    std::vector<Expr *> args;

    CallExpr(Expr *callee, Token *rParen, std::vector<Expr *> args);

    virtual std::vector<Expr *> getArguments(Compiler *compiler);
    virtual FunctionEntry *getFunctionEntry(Compiler *compiler);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

protected:
    TypeEntry *computeReturnType(Compiler *compiler) override;

    std::vector<TypeEntry *> getArgumentsTypes(Compiler *compiler);
};


#endif //RISOTTOV2_CALLEXPR_H
