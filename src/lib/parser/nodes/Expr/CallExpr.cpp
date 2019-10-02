//
// Created by rvigee on 10/2/19.
//

#include "CallExpr.h"

CallExpr::CallExpr(Token *identifier, Token *rParen, std::vector<Expr *> args): identifier(identifier), rParen(rParen), args(args) {

}
