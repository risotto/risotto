//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_FUNCTIONSTMT_H
#define RISOTTOV2_FUNCTIONSTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/ParameterDefinition.h>
#include <lib/compiler/FunctionsTable.h>
#include <vector>
#include <lib/parser/nodes/TypeDescriptor.h>

class FunctionStmt : public Stmt {
public:
    Token *type;
    ParameterDefinition *receiver;
    Token *name;
    std::vector<TypeDescriptor> returnTypes;
    std::vector<ParameterDefinition> parameters;
    std::vector<Stmt *> body;
    Token *closeBlock;

    bool autoRegister = true;

    FunctionStmt(Token *type, ParameterDefinition *receiver, Token *name, std::vector<TypeDescriptor> returnTypes,
                 std::vector<ParameterDefinition> parameters, std::vector<Stmt *> body,
                 Token *closeBlock);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    FunctionEntry *getFunctionEntry(Compiler *compiler);
private:
    FunctionEntry *_functionEntry = nullptr;
};


#endif //RISOTTOV2_FUNCTIONSTMT_H
