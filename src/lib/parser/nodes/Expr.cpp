//
// Created by rvigee on 10/2/19.
//

#include "Expr.h"
#include "lib/compiler/ReturnTypes.h"

ReturnTypes Expr::getReturnType(Compiler *compiler) {
    if (_isReturnTypesInit) {
        return _returnTypes;
    }

    _returnTypes = computeReturnType(compiler);
    _isReturnTypesInit = true;

    return _returnTypes;
}

