//
// Created by rvigee on 10/7/19.
//

#include "FunctionExpr.h"
#include "lib/compiler/TypeReference.h"
#include "lib/compiler/ReturnTypes.h"
#include "lib/compiler/TypeDefinition.h"

FunctionExpr::FunctionExpr(FunctionStmt *functionStmt) : functionStmt(functionStmt) {}

ReturnTypes FunctionExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = functionStmt->getFunctionEntry(compiler);

    return new FunctionTypeReference(new FunctionTypeDefinition(functionEntry));
}

std::vector<ByteResolver *> FunctionExpr::compile(Compiler *compiler) {
    return functionStmt->compile(compiler);
}

