//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_WHILESTMT_H
#define RISOTTOV2_WHILESTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/nodes/Expr.h>
#include "BlockStmt.h"

class WhileStmt: public Stmt {
public:
    Expr *condition;
    BlockStmt *body;

    WhileStmt(Expr *condition, BlockStmt *body);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
};


#endif //RISOTTOV2_WHILESTMT_H
