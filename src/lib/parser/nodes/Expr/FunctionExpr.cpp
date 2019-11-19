//
// Created by rvigee on 10/7/19.
//

#include <lib/compiler/utils/Utils.h>
#include "FunctionExpr.h"
#include "lib/compiler/ReturnTypes.h"
#include "lib/compiler/TypeDefinition.h"

FunctionExpr::FunctionExpr(FunctionStmt *functionStmt) : functionStmt(functionStmt) {}

ReturnTypes FunctionExpr::computeReturnType(Compiler *compiler) {
    auto functionEntry = functionStmt->getFunctionEntry(compiler);

    return new FunctionTypeDescriptor(functionEntry);
}

std::vector<ByteResolver *> FunctionExpr::compile(Compiler *compiler) {
    auto bytes = functionStmt->compile(compiler);

    Utils::loadFunctionEntryAddr(compiler, functionStmt->getFunctionEntry(compiler), bytes);

    return bytes;
}

void FunctionExpr::symbolize(Compiler *compiler) {
    functionStmt->symbolize(compiler);
}
