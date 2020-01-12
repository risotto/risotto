//
// Created by rvigee on 1/7/20.
//

#include "SubscriptExpr.h"

SubscriptExpr::SubscriptExpr(Expr *expr, Token *rParen, const std::vector<Expr *>& args): GetCallExpr(expr, rParen, rParen, rParen, args) {

}

bool SubscriptExpr::isArgumentReference(Compiler *compiler, unsigned int i) {
    return false;
}

bool SubscriptExpr::loadCallAddr(Compiler *compiler, std::vector<ByteResolver *> &bytes) {
    return Utils::loadFunctionEntryAddr(compiler, getFunctionEntry(compiler), bytes);
}

ReturnTypes SubscriptExpr::computeReturnType(Compiler *compiler) {
    auto entry = getFunctionEntry(compiler);

    return entry->descriptor->returnTypes;
}

FunctionEntry *SubscriptExpr::getFunctionEntry(Compiler *compiler) {
    auto calleeType = callee->getReturnType(compiler);

    if (!calleeType.single()) {
        throw CompilerError("Return type has to be single", rParen->position);
    }

    auto def = calleeType[0]->getTypeDefinition();
    auto argsTypes = getArgumentsTypes(compiler);
    auto entry = def->operators.find("[]", argsTypes);

    return entry;
}
