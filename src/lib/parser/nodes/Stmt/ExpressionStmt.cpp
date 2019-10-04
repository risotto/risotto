//
// Created by rvigee on 10/2/19.
//

#include "ExpressionStmt.h"

ExpressionStmt::ExpressionStmt(Expr *expr): expr(expr) {

}

std::vector<ByteResolver *> ExpressionStmt::compile(Compiler *compiler) {
    return expr->compile(compiler);
}
