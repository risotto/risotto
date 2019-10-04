//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_FUNCTIONSTMT_H
#define RISOTTOV2_FUNCTIONSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/ParameterDefinition.h>
#include <vector>

class FunctionStmt : public Stmt {
public:
    Token *name;
    Token *returnType;
    std::vector<ParameterDefinition> parameters;
    std::vector<Stmt *> body;
    Token *closeBlock;

    FunctionStmt(Token *name, Token *returnType, std::vector<ParameterDefinition> parameters, std::vector<Stmt *> body,
                 Token *closeBlock);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;
};


#endif //RISOTTOV2_FUNCTIONSTMT_H
