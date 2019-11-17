//
// Created by rvigee on 11/16/19.
//

#ifndef RISOTTOV2_COMPILERWALKER_H
#define RISOTTOV2_COMPILERWALKER_H


#include <lib/parser/nodes/ASTWalker.h>

class CompilerWalker: public ASTWalker {
protected:
    void walkBinaryExpr(BinaryExpr *expr) override;

    void walkReturnStmt(ReturnStmt *stmt) override;

    void walkWhileStmt(WhileStmt *stmt) override;

    void walkTypeStmt(TypeStmt *stmt) override;

    void walkSetExpr(SetExpr *expr) override;

    void walkCallExpr(CallExpr *expr) override;

    void walkNewCallExpr(NewCallExpr *expr) override;

    void walkMixedCallExpr(MixedCallExpr *expr) override;

    void walkLogicalExpr(LogicalExpr *expr) override;

    void walkBlockStmt(BlockStmt *stmt) override;

    void walkExpressionStmt(ExpressionStmt *stmt) override;

    void walkVarDeclStmt(VarDeclStmt *stmt) override;

    void walkUnaryExpr(UnaryExpr *expr) override;

    void walkFunctionExpr(FunctionExpr *expr) override;

    void walkFunctionStmt(FunctionStmt *stmt) override;

    void walkGetCallExpr(GetCallExpr *expr) override;

    void walkIdentifierCallExpr(IdentifierCallExpr *expr) override;

    void walkIfStmt(IfStmt *stmt) override;

    void walkGroupingExpr(GroupingExpr *expr) override;

    void walkIdentifierExpr(IdentifierExpr *expr) override;

    void walkLiteralExpr(LiteralExpr *expr) override;

    void walkArrayExpr(ArrayExpr *expr) override;

    void walkGetExpr(GetExpr *expr) override;
};


#endif //RISOTTOV2_COMPILERWALKER_H
