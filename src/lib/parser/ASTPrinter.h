//
// Created by rvigee on 10/3/19.
//

#ifndef RISOTTOV2_ASTPRINTER_H
#define RISOTTOV2_ASTPRINTER_H


#include <lib/parser/nodes/Stmt.h>
#include <vector>
#include <iterator>
#include <lib/parser/nodes/Stmt/FunctionStmt.h>
#include <lib/parser/nodes/Stmt/IfStmt.h>
#include <lib/parser/nodes/Stmt/ReturnStmt.h>
#include <lib/parser/nodes/Expr/BinaryExpr.h>
#include <lib/parser/nodes/Expr/IdentifierExpr.h>
#include <lib/parser/nodes/Expr/LiteralExpr.h>
#include <lib/parser/nodes/Expr/GroupingExpr.h>
#include <lib/parser/nodes/Stmt/BlockStmt.h>
#include <lib/parser/nodes/Expr/CallExpr.h>
#include <lib/parser/nodes/Stmt/ExpressionStmt.h>
#include <lib/parser/nodes/Expr/GetExpr.h>
#include <lib/parser/nodes/Expr/VarDeclStmt.h>
#include <lib/parser/nodes/Expr/SetExpr.h>
#include <lib/parser/nodes/Stmt/WhileStmt.h>
#include <lib/parser/nodes/Expr/UnaryExpr.h>
#include <lib/parser/nodes/Expr/ArrayExpr.h>
#include <lib/parser/nodes/Stmt/TypeStmt.h>

namespace ASTPrinter {
    std::string indent(std::string str, int n);

    template<typename T>
    std::string print(T stmt);

    template<>
    std::string print<Stmt *>(Stmt * stmt);

    template<>
    std::string print<Expr *>(Expr * stmt);

    template<>
    std::string print<std::vector<Stmt *>>(std::vector<Stmt *> stmts);

    template<>
    std::string print<std::vector<Expr *>>(std::vector<Expr *> stmts);

    template<>
    std::string print<FunctionStmt *>(FunctionStmt *stmt);

    template<>
    std::string print<IfStmt *>(IfStmt *stmt);

    template<>
    std::string print<ReturnStmt *>(ReturnStmt *stmt);

    template<>
    std::string print<BinaryExpr *>(BinaryExpr *stmt);

    template<>
    std::string print<UnaryExpr *>(UnaryExpr *stmt);

    template<>
    std::string print<IdentifierExpr *>(IdentifierExpr *stmt);

    template<>
    std::string print<LiteralExpr *>(LiteralExpr *stmt);

    template<>
    std::string print<GroupingExpr *>(GroupingExpr *stmt);

    template<>
    std::string print<BlockStmt *>(BlockStmt *stmt);

    template<>
    std::string print<CallExpr *>(CallExpr *stmt);

    template<>
    std::string print<ExpressionStmt *>(ExpressionStmt *stmt);

    template<>
    std::string print<GetExpr *>(GetExpr *stmt);

    template<>
    std::string print<VarDeclStmt *>(VarDeclStmt *stmt);

    template<>
    std::string print<SetExpr *>(SetExpr *stmt);

    template<>
    std::string print<WhileStmt *>(WhileStmt *stmt);

    template<>
    std::string print<ArrayExpr *>(ArrayExpr *stmt);

    template<>
    std::string print<TypeStmt *>(TypeStmt *stmt);
};


#endif //RISOTTOV2_ASTPRINTER_H
