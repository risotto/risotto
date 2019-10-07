//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_IFSTMT_H
#define RISOTTOV2_IFSTMT_H


#include <lib/parser/nodes/Expr.h>
#include <lib/parser/nodes/Stmt.h>

class IfStmt: public Stmt {
public:
    Expr *condition;
    Stmt *thenBranch;
    std::vector<IfStmt *> elseifs;
    Stmt *elseBranch;

    IfStmt(Expr *condition, Stmt *thenBranch, std::vector<IfStmt *> elseifs, Stmt *elseBranch);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
};


#endif //RISOTTOV2_IFSTMT_H
