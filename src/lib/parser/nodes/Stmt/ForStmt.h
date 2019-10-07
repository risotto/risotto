//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_FORSTMT_H
#define RISOTTOV2_FORSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include "BlockStmt.h"
#include "WhileStmt.h"

class ForStmt : public BlockStmt {
public:
    ForStmt(Stmt *initializer, WhileStmt *condition);
};


#endif //RISOTTOV2_FORSTMT_H
