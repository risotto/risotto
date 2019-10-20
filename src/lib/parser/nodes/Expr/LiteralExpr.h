//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_LITERALEXPR_H
#define RISOTTOV2_LITERALEXPR_H


#include <lib/tokenizer/Token.h>
#include <lib/parser/nodes/Expr.h>
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"

class LiteralExpr: public Expr {
public:
    Token *value;

    explicit LiteralExpr(Token *value);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

protected:
    std::string getReturnTypeName();
    TypeDefinition *getReturnTypeDefinition(Compiler *compiler);
    ReturnTypes computeReturnType(Compiler *compiler) override;
};


#endif //RISOTTOV2_LITERALEXPR_H
