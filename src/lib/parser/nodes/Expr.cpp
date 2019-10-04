//
// Created by rvigee on 10/2/19.
//

#include "Expr.h"

TypeEntry *Expr::getReturnType(Compiler *compiler) {
    if (_returnType) {
        return _returnType;
    }

    _returnType = computeReturnType(compiler);

    return _returnType;
}

TypeEntry *Expr::computeReturnType(Compiler *compiler) {
    throw std::logic_error("computeReturnType must be implemented");
}
