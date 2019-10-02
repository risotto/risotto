//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_RETURNSTMT_H
#define RISOTTOV2_RETURNSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>

class ReturnStmt : public Stmt {
public:
    Token *keyword;
    Expr *value;

    ReturnStmt(Token *keyword, Expr *value);
};


#endif //RISOTTOV2_RETURNSTMT_H
