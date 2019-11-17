//
// Created by rvigee on 11/16/19.
//

#ifndef RISOTTOV2_ASTWALKER_H
#define RISOTTOV2_ASTWALKER_H

#define NODES(m) \
    m(BlockStmt) \
    m(ExpressionStmt) \
    m(FunctionStmt) \
    m(IfStmt) \
    m(ReturnStmt) \
    m(TypeStmt) \
    m(WhileStmt) \
    m(ArrayExpr) \
    m(BinaryExpr) \
    m(CallExpr) \
    m(FunctionExpr) \
    m(GetCallExpr) \
    m(GetExpr) \
    m(GroupingExpr) \
    m(IdentifierCallExpr) \
    m(IdentifierExpr) \
    m(LiteralExpr) \
    m(LogicalExpr) \
    m(MixedCallExpr) \
    m(NewCallExpr) \
    m(SetExpr) \
    m(UnaryExpr) \
    m(VarDeclStmt)

#define NODE_FUNC(n) virtual void walk##n(n *) = 0;
#define FORWARD_DECL(n) class n;

NODES(FORWARD_DECL)

class ASTWalker {
public:
    NODES(NODE_FUNC)
};


#endif //RISOTTOV2_ASTWALKER_H
