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
    const Token *keyword;
    std::vector<Expr *> values;

    ReturnStmt(const Token *keyword, std::vector<Expr *> values);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;
};


#endif //RISOTTOV2_RETURNSTMT_H
