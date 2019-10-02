//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_PRINTSTMT_H
#define RISOTTOV2_PRINTSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/nodes/Expr.h>

class PrintStmt: public Stmt {
public:
    Expr *value;

    explicit PrintStmt(Expr *value);
};


#endif //RISOTTOV2_PRINTSTMT_H
