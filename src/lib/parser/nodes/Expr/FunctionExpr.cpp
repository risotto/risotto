//
// Created by rvigee on 10/7/19.
//

#include "FunctionExpr.h"
#include "lib/compiler/TypeReference.h"

FunctionExpr::FunctionExpr(FunctionStmt *functionStmt) : functionStmt(functionStmt) {}

TypesEntries FunctionExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = functionStmt->getFunctionEntry(compiler);

    return new FunctionTypeEntry("", functionEntry);
}

std::vector<ByteResolver *> FunctionExpr::compile(Compiler *compiler) {
    return functionStmt->compile(compiler);
}

