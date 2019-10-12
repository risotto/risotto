//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_LOGICALEXPR_H
#define RISOTTOV2_LOGICALEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"

class LogicalExpr: public Expr {
public:
    Expr *left;
    Token *op;
    Expr *right;

    LogicalExpr(Expr *left, Token *op, Expr *right);

    std::vector<ByteResolver *>
    generateOr(std::vector<ByteResolver *> leftBytes, std::vector<ByteResolver *> rightBytes, Compiler *compiler);

    std::vector<ByteResolver *>
    generateAnd(std::vector<ByteResolver *> leftBytes, std::vector<ByteResolver *> rightBytes, Compiler *compiler);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_LOGICALEXPR_H
