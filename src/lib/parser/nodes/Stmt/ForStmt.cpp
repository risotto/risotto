//
// Created by rvigee on 10/7/19.
//

#include "ForStmt.h"

ForStmt::ForStmt(Stmt *initializer, WhileStmt *condition): BlockStmt({initializer, condition}) {

}
