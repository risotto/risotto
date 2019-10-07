//
// Created by rvigee on 10/2/19.
//

#include "GroupingExpr.h"
#include "lib/compiler/TypeReference.h"

GroupingExpr::GroupingExpr(Expr *expr, Token *lParen, Token *rParen): expr(expr), lParen(lParen), rParen(rParen) {

}

std::vector<ByteResolver *> GroupingExpr::compile(Compiler *compiler) {
    return expr->compile(compiler);
}

TypesEntries GroupingExpr::computeReturnType(Compiler *compiler) {
    return expr->getReturnType(compiler);
}
