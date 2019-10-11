//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_EXPR_H
#define RISOTTOV2_EXPR_H


#include <lib/compiler/TypesTable.h>
#include "Node.h"
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"
#include <vector>

class Expr : public Node {
private:
    bool _isReturnTypesInit = false;
    ReturnTypes _returnTypes;

protected:
    virtual ReturnTypes computeReturnType(Compiler *compiler) = 0;

public:
    ReturnTypes getReturnType(Compiler *compiler);
};


#endif //RISOTTOV2_EXPR_H
