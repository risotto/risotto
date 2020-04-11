//
// Created by rvigee on 10/2/19.
//

#include "GroupingExpr.h"
#include "lib/compiler/ReturnTypes.h"

GroupingExpr::GroupingExpr(Expr *expr, const Token *lParen, const Token *rParen): expr(expr), lParen(lParen), rParen(rParen) {

}

std::vector<ByteResolver *> GroupingExpr::compile(Compiler *compiler) {
    return expr->compile(compiler);
}

ReturnTypes GroupingExpr::computeReturnType(Compiler *compiler) {
    return expr->getReturnType(compiler);
}

void GroupingExpr::symbolize(Compiler *compiler) {
    expr->symbolize(compiler);
}
