//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_EXPR_H
#define RISOTTOV2_EXPR_H


#include <lib/compiler/TypesTable.h>
#include "Node.h"

class Expr : public Node {
private:
    TypeEntry *_returnType = nullptr;

protected:
    virtual TypeEntry *computeReturnType(Compiler *compiler);

public:
    virtual ~Expr() = default;

    TypeEntry *getReturnType(Compiler *compiler);
};


#endif //RISOTTOV2_EXPR_H
