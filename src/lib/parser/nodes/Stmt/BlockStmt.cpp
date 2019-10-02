//
// Created by rvigee on 10/2/19.
//

#include "BlockStmt.h"

#include <utility>

BlockStmt::BlockStmt(std::vector<Stmt *> stmts): stmts(std::move(stmts)) {

}
