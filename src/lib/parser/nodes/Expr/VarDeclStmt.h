//
// Created by rvigee on 10/6/19.
//

#ifndef RISOTTOV2_VARDECLSTMT_H
#define RISOTTOV2_VARDECLSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/nodes/Expr.h>

class VarDeclStmt: public Stmt {
public:
    std::vector<Token *> identifiers;
    Token *op;
    Expr *value;

    VarDeclStmt(std::vector<Token *> identifiers, Token *op, Expr *value);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
};


#endif //RISOTTOV2_VARDECLSTMT_H
