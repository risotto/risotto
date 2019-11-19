//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_EXPRESSIONSTMT_H
#define RISOTTOV2_EXPRESSIONSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/nodes/Expr.h>

class ExpressionStmt: public Stmt {
public:
    Expr *expr;

    explicit ExpressionStmt(Expr *expr);

    std::vector<ByteResolver *> compile(Compiler *compiler);

    void symbolize(Compiler *compiler) override;
};


#endif //RISOTTOV2_EXPRESSIONSTMT_H
