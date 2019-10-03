//
// Created by rvigee on 10/3/19.
//

#include "ASTPrinter.h"

#include <lib/parser/nodes/Stmt/FunctionStmt.h>
#include <iostream>
#include <sstream>
#include <lib/parser/nodes/Stmt/PrintStmt.h>
#include <lib/parser/nodes/Expr/LiteralExpr.h>

std::vector<std::string> split(std::string s, char delimiter) {
    std::vector<std::string> lines;

    std::stringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        lines.push_back(token);
    }

    return lines;
}

#include <utility>

std::string repeat(const std::string& input, size_t num)
{
    std::ostringstream os;
    std::fill_n(std::ostream_iterator<std::string>(os), num, input);
    return os.str();
}

std::string ASTPrinter::indent(std::string str, int n) {
    std::vector<std::string> lines = split(str, '\n');

    std::stringstream ss;

    for (const auto &line : lines) {
        if (ss.str().length() > 0) {
            ss << std::endl;
        }

        ss << repeat("  ", n) << line;
    }

    if (str.at(str.length() - 1) == '\n') {
        ss << std::endl;
    }

    return ss.str();
}

template<typename T>
std::string ASTPrinter::print(T t) {
    return "???\n";
    throw std::logic_error("Unhandled stmt");
}

#define _DC(T, V) \
if (auto V = dynamic_cast<T *>(stmt)) { \
  return print(V); \
}

#define DC(T) _DC(T, __##T)

template<>
std::string ASTPrinter::print<Stmt *>(Stmt *stmt) {
    DC(FunctionStmt)
    DC(PrintStmt)
    DC(IfStmt)
    DC(BlockStmt)
    DC(ReturnStmt)

    return print(dynamic_cast<Node *>(stmt));
}

template<>
std::string ASTPrinter::print<Expr *>(Expr *stmt) {
    DC(BinaryExpr)
    DC(VariableExpr)
    DC(LiteralExpr)
    DC(CallExpr)
    DC(GroupingExpr)

    return print(dynamic_cast<Node *>(stmt));
}

template<>
std::string ASTPrinter::print<std::vector<Stmt *>>(std::vector<Stmt *> stmts) {
    if (stmts.empty()) {
        return "<empty>\n";
    }

    std::stringstream ss;

    for (auto stmt : stmts) {
        ss << print(stmt);
    }

    return ss.str();
}

template<>
std::string ASTPrinter::print<std::vector<Expr *>>(std::vector<Expr *> stmts) {
    if (stmts.empty()) {
        return "<empty>\n";
    }

    std::stringstream ss;

    for (auto stmt : stmts) {
        ss << print(stmt);
    }

    return ss.str();
}

template<>
std::string ASTPrinter::print<FunctionStmt *>(FunctionStmt *stmt) {
    std::stringstream ss;

    ss << "FunctionStmt" << std::endl;

    ss << indent(print(stmt->body), 1);

    return ss.str();
}

template<>
std::string ASTPrinter::print<PrintStmt *>(PrintStmt *stmt) {
    std::stringstream ss;

    ss << "PrintStmt" << std::endl;
    ss << indent(print(stmt->value), 1);

    return ss.str();
}

template<>
std::string ASTPrinter::print<ReturnStmt *>(ReturnStmt *stmt) {
    std::stringstream ss;

    ss << "ReturnStmt" << std::endl;
    ss << indent(print(stmt->value), 1);

    return ss.str();
}

template<>
std::string ASTPrinter::print<IfStmt *>(IfStmt *stmt) {
    std::stringstream ss;

    ss << "IfStmt" << std::endl;
    ss << indent("+If:", 1) << std::endl;
    ss << indent(print(stmt->condition), 2);
    ss << indent("+Then:", 1) << std::endl;
    ss << indent(print(stmt->thenBranch), 2);

    return ss.str();
}

template<>
std::string ASTPrinter::print<BinaryExpr *>(BinaryExpr *stmt) {
    std::stringstream ss;

    ss << "BinaryExpr" << std::endl;
    ss << indent("+Left:", 1) << std::endl;
    ss << indent(print(stmt->left), 2);
    ss << indent("+Op: " + stmt->op->lexeme, 1) << std::endl;
    ss << indent("+Right:", 1) << std::endl;
    ss << indent(print(stmt->right), 2);

    return ss.str();
}

template<>
std::string ASTPrinter::print<VariableExpr *>(VariableExpr *stmt) {
    std::stringstream ss;

    ss << "VariableExpr" << std::endl;
    ss << indent("+Name: " + stmt->name->lexeme, 1) << std::endl;

    return ss.str();
}

template<>
std::string ASTPrinter::print<LiteralExpr *>(LiteralExpr *stmt) {
    std::stringstream ss;

    ss << "LiteralExpr" << std::endl;
    ss << indent("+Value: " + stmt->value->lexeme, 1) << std::endl;

    return ss.str();
}

template<>
std::string ASTPrinter::print<GroupingExpr *>(GroupingExpr *stmt) {
    std::stringstream ss;

    ss << "GroupingExpr" << std::endl;
    ss << indent(print(stmt->expr), 1);

    return ss.str();
}

template<>
std::string ASTPrinter::print<BlockStmt *>(BlockStmt *stmt) {
    std::stringstream ss;

    ss << "BlockStmt" << std::endl;
    ss << indent(print(stmt->stmts), 1);

    return ss.str();
}

template<>
std::string ASTPrinter::print<CallExpr *>(CallExpr *stmt) {
    std::stringstream ss;

    ss << "CallExpr" << std::endl;
    ss << indent("+Name: " + stmt->identifier->lexeme, 1) << std::endl;
    ss << indent("+Args:", 1) << std::endl;
    ss << indent(print(stmt->args), 1);

    return ss.str();
}