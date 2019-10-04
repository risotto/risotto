//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_BLOCKSTMT_H
#define RISOTTOV2_BLOCKSTMT_H

#include "lib/parser/nodes/Stmt.h"
#include <vector>

class BlockStmt: public Stmt {
public:
    std::vector<Stmt *> stmts;

    explicit BlockStmt(std::vector<Stmt *> stmts);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
};


#endif //RISOTTOV2_BLOCKSTMT_H
