//
// Created by rvigee on 11/16/19.
//

#include "CompilerWalker.h"
#include <iostream>

void CompilerWalker::walkBinaryExpr(BinaryExpr *expr) {
    std::cout << "BinaryExpr" << std::endl;
}

void CompilerWalker::walkReturnStmt(ReturnStmt *stmt) {
    std::cout << "ReturnStmt" << std::endl;
}

void CompilerWalker::walkWhileStmt(WhileStmt *stmt) {
    std::cout << "WhileStmt" << std::endl;
}

void CompilerWalker::walkTypeStmt(TypeStmt *stmt) {
    std::cout << "TypeStmt" << std::endl;
}

void CompilerWalker::walkSetExpr(SetExpr *expr) {
    std::cout << "SetExpr" << std::endl;
}

void CompilerWalker::walkCallExpr(CallExpr *expr) {
    std::cout << "CallExpr" << std::endl;
}

void CompilerWalker::walkNewCallExpr(NewCallExpr *expr) {
    std::cout << "NewCallExpr" << std::endl;
}

void CompilerWalker::walkMixedCallExpr(MixedCallExpr *expr) {
    std::cout << "MixedCallExpr" << std::endl;
}

void CompilerWalker::walkLogicalExpr(LogicalExpr *expr) {
    std::cout << "LogicalExpr" << std::endl;
}

void CompilerWalker::walkBlockStmt(BlockStmt *stmt) {
    std::cout << "BlockStmt" << std::endl;
}

void CompilerWalker::walkExpressionStmt(ExpressionStmt *stmt) {
    std::cout << "ExpressionStmt" << std::endl;
}

void CompilerWalker::walkVarDeclStmt(VarDeclStmt *stmt) {
    std::cout << "VarDeclStmt" << std::endl;
}

void CompilerWalker::walkUnaryExpr(UnaryExpr *expr) {
    std::cout << "UnaryExpr" << std::endl;
}

void CompilerWalker::walkFunctionExpr(FunctionExpr *expr) {
    std::cout << "FunctionExpr" << std::endl;
}

void CompilerWalker::walkFunctionStmt(FunctionStmt *stmt) {
    std::cout << "FunctionStmt" << std::endl;
}

void CompilerWalker::walkGetCallExpr(GetCallExpr *expr) {
    std::cout << "GetCallExpr" << std::endl;
}

void CompilerWalker::walkIdentifierCallExpr(IdentifierCallExpr *expr) {
    std::cout << "IdentifierCallExpr" << std::endl;
}

void CompilerWalker::walkIfStmt(IfStmt *stmt) {
    std::cout << "IfStmt" << std::endl;
}

void CompilerWalker::walkGroupingExpr(GroupingExpr *expr) {
    std::cout << "GroupingExpr" << std::endl;
}

void CompilerWalker::walkIdentifierExpr(IdentifierExpr *expr) {
    std::cout << "IdentifierExpr" << std::endl;
}

void CompilerWalker::walkLiteralExpr(LiteralExpr *expr) {
    std::cout << "LiteralExpr" << std::endl;
}

void CompilerWalker::walkArrayExpr(ArrayExpr *expr) {
    std::cout << "ArrayExpr" << std::endl;
}

void CompilerWalker::walkGetExpr(GetExpr *expr) {
    std::cout << "GetExpr" << std::endl;
}
