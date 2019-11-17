//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_FUNCTIONEXPR_H
#define RISOTTOV2_FUNCTIONEXPR_H


#include <lib/parser/nodes/Expr.h>
#include <lib/parser/nodes/Stmt/FunctionStmt.h>
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"

class FunctionExpr: public Expr {
public:
    FunctionStmt *functionStmt;

    FunctionExpr(FunctionStmt *functionStmt);
public:
    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void walk(ASTWalker *walker) override;

protected:
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_FUNCTIONEXPR_H
