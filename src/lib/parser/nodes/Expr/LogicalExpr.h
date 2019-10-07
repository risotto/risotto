//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_LOGICALEXPR_H
#define RISOTTOV2_LOGICALEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/tokenizer/Token.h>

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
    TypesEntries computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_LOGICALEXPR_H
