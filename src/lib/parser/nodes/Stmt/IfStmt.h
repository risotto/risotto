//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_IFSTMT_H
#define RISOTTOV2_IFSTMT_H


#include <lib/parser/nodes/Expr.h>
#include <lib/parser/nodes/Stmt.h>

class IfBranch {
public:
    Expr *condition;
    Stmt *body;

    std::vector<ByteResolver *> bytes;

public:
    IfBranch(Expr *condition, Stmt *body);
};

class IfStmt : public Stmt {
public:
    Expr *condition;
    Stmt *thenBranch = nullptr;
    std::vector<IfStmt *> elseifs;
    Stmt *elseBranch = nullptr;

    std::vector<IfBranch *> branches;

    IfStmt(Expr *condition, Stmt *thenBranch, std::vector<IfStmt *> elseifs, Stmt *elseBranch);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;

private:
    ByteResolver *generateExitByte(IfBranch *pBranch);

    ByteResolver *generateNextBranchByte(IfBranch *branch);
};


#endif //RISOTTOV2_IFSTMT_H
