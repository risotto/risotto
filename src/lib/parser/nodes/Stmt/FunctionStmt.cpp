//
// Created by rvigee on 10/2/19.
//

#include "FunctionStmt.h"

FunctionStmt::FunctionStmt(
        Token *name,
        Token *returnType,
        std::vector<ParameterDefinition> parameters,
        std::vector<Stmt *> body,
        Token *closeBlock
): name(name), returnType(returnType), parameters(parameters), body(body), closeBlock(closeBlock) {

}
