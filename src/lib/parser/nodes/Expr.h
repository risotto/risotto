//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_EXPR_H
#define RISOTTOV2_EXPR_H


#include <lib/compiler/TypesTable.h>
#include "Node.h"
#include <vector>

class Expr : public Node {
private:
    bool _isReturnTypesInit = false;
    TypesEntries _returnTypes;

protected:
    virtual TypesEntries computeReturnType(Compiler *compiler);

public:
    virtual ~Expr() = default;

    TypesEntries getReturnType(Compiler *compiler);
};


#endif //RISOTTOV2_EXPR_H
