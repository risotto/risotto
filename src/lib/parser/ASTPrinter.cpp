//
// Created by rvigee on 10/3/19.
//

#include "ASTPrinter.h"

#include <iostream>
#include <sstream>
#include <lib/parser/nodes/Stmt/FunctionStmt.h>
#include <lib/parser/nodes/Expr/LiteralExpr.h>
#include <lib/parser/nodes/Expr/BinaryExpr.h>

namespace ASTPrinter {
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
#include <lib/parser/nodes/Stmt/ExpressionStmt.h>

    std::string repeat(const std::string &input, size_t num) {
        std::ostringstream os;
        std::fill_n(std::ostream_iterator<std::string>(os), num, input);
        return os.str();
    }

    std::string indent(std::string str, int n) {
        std::vector<std::string> lines = split(str, '\n');

        std::stringstream ss;

        for (const auto &line : lines) {
            if (ss.str().length() > 0) {
                ss << std::endl;
            }

            ss << repeat("· ", n) << line;
        }

        if (str.at(str.length() - 1) == '\n') {
            ss << std::endl;
        }

        return ss.str();
    }

    template<typename T>
    std::string print(T t) {
        return "???\n";
        throw std::logic_error("Unhandled stmt");
    }

#define _DC(T, V) \
if (auto V = dynamic_cast<T *>(stmt)) { \
  return print(V); \
}

#define DC(T) _DC(T, __##T)

    template<>
    std::string print<Stmt *>(Stmt *stmt) {
        DC(FunctionStmt)
        DC(IfStmt)
        DC(BlockStmt)
        DC(ReturnStmt)
        DC(ExpressionStmt)
        DC(VarDeclStmt)
        DC(WhileStmt)
        DC(TypeStmt)

        return print(dynamic_cast<Node *>(stmt));
    }

    template<>
    std::string print<Expr *>(Expr *stmt) {
        DC(BinaryExpr)
        DC(UnaryExpr)
        DC(IdentifierExpr)
        DC(LiteralExpr)
        DC(GroupingExpr)
        DC(GetExpr)
        DC(SetExpr)
        DC(ArrayExpr)
        DC(IdentifierCallExpr)
        DC(GetCallExpr)
        DC(FunctionExpr)

        return print(dynamic_cast<Node *>(stmt));
    }

    template<>
    std::string print<std::vector<Stmt *>>(std::vector<Stmt *> stmts) {
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
    std::string print<std::vector<Expr *>>(std::vector<Expr *> stmts) {
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
    std::string print<FunctionStmt *>(FunctionStmt *stmt) {
        std::stringstream ss;

        ss << "FunctionStmt" << std::endl;
        ss << indent("+Type: " + stmt->type->lexeme, 1) << std::endl;
        if (stmt->receiver) {
            ss << indent("+Receiver: " + stmt->receiver->type->toString(), 1) << std::endl;
        }
        if (stmt->name) {
            ss << indent("+Name: " + stmt->name->lexeme, 1) << std::endl;
        }
        ss << indent("+Body: ", 1) << std::endl;
        ss << indent(print(stmt->body), 2);

        return ss.str();
    }

    template<>
    std::string print<ReturnStmt *>(ReturnStmt *stmt) {
        std::stringstream ss;

        ss << "ReturnStmt" << std::endl;
        ss << indent("+Values:", 1) << std::endl;
        ss << indent(print(stmt->values), 2);

        return ss.str();
    }

    template<>
    std::string print<IfStmt *>(IfStmt *stmt) {
        std::stringstream ss;

        ss << "IfStmt" << std::endl;
        ss << indent("+If:", 1) << std::endl;
        ss << indent(print(stmt->condition), 2);
        ss << indent("+Then:", 1) << std::endl;
        ss << indent(print(stmt->thenBranch), 2);

        return ss.str();
    }

    template<>
    std::string print<BinaryExpr *>(BinaryExpr *stmt) {
        std::stringstream ss;

        ss << "BinaryExpr" << std::endl;
        ss << indent("+Left:", 1) << std::endl;
        ss << indent(print(stmt->left()), 2);
        ss << indent("+Op: " + stmt->op()->lexeme, 1) << std::endl;
        ss << indent("+Right:", 1) << std::endl;
        ss << indent(print(stmt->right()), 2);

        return ss.str();
    }

    template<>
    std::string print<UnaryExpr *>(UnaryExpr *stmt) {
        std::stringstream ss;

        ss << "UnaryExpr" << std::endl;
        ss << indent("+Op: " + stmt->op()->lexeme, 1) << std::endl;
        ss << indent("+Right:", 1) << std::endl;
        ss << indent(print(stmt->right()), 2);

        return ss.str();
    }

    template<>
    std::string print<IdentifierExpr *>(IdentifierExpr *stmt) {
        std::stringstream ss;

        ss << "IdentifierExpr" << std::endl;
        ss << indent("+Name: " + stmt->name->lexeme, 1) << std::endl;

        return ss.str();
    }

    template<>
    std::string print<LiteralExpr *>(LiteralExpr *stmt) {
        std::stringstream ss;

        ss << "LiteralExpr" << std::endl;
        ss << indent("+Value: " + stmt->value->lexeme, 1) << std::endl;

        return ss.str();
    }

    template<>
    std::string print<GroupingExpr *>(GroupingExpr *stmt) {
        std::stringstream ss;

        ss << "GroupingExpr" << std::endl;
        ss << indent(print(stmt->expr), 1);

        return ss.str();
    }

    template<>
    std::string print<BlockStmt *>(BlockStmt *stmt) {
        std::stringstream ss;

        ss << "BlockStmt" << std::endl;
        ss << indent(print(stmt->stmts), 1);

        return ss.str();
    }

    template<>
    std::string print<ExpressionStmt *>(ExpressionStmt *stmt) {
        std::stringstream ss;

        ss << "ExpressionStmt" << std::endl;
        ss << indent(print(stmt->expr), 1) << std::endl;

        return ss.str();
    }

    template<>
    std::string print<GetExpr *>(GetExpr *stmt) {
        std::stringstream ss;

        ss << "GetExpr" << std::endl;
        ss << indent("+Callee:", 1) << std::endl;
        ss << indent(print(stmt->callee), 2);
        ss << indent("+Identifier: " + stmt->identifier->lexeme, 1) << std::endl;

        return ss.str();
    }

    template<>
    std::string print<VarDeclStmt *>(VarDeclStmt *stmt) {
        std::stringstream ss;

        ss << "VarDeclStmt" << std::endl;
        ss << indent("+Identifiers:", 1) << std::endl;
        for (auto id : stmt->identifiers) {
            ss << indent("- " + id.first->lexeme + ": ", 2);
            if (id.second == nullptr) {
                ss << "<inferred>";
            } else {
                ss << id.second->toString();
            }
            ss << std::endl;
        }
        ss << indent("+Value:", 1) << std::endl;
        ss << indent(print(stmt->value), 2);

        return ss.str();
    }

    template<>
    std::string print<SetExpr *>(SetExpr *stmt) {
        std::stringstream ss;

        ss << "SetExpr" << std::endl;
        ss << indent("+Left:", 1) << std::endl;
        ss << indent(print(stmt->left), 2);
        ss << indent("+Right:", 1) << std::endl;
        ss << indent(print(stmt->right), 2);

        return ss.str();
    }

    template<>
    std::string print<WhileStmt *>(WhileStmt *stmt) {
        std::stringstream ss;

        ss << "WhileStmt" << std::endl;
        ss << indent("+Condition:", 1) << std::endl;
        ss << indent(print(stmt->condition), 2);
        ss << indent("+Body:", 1) << std::endl;
        ss << indent(print(stmt->body), 2);

        return ss.str();
    }

    template<>
    std::string print<ArrayExpr *>(ArrayExpr *stmt) {
        std::stringstream ss;

        ss << "ArrayExpr" << std::endl;
        ss << indent("+Type: ", 1) + stmt->type->toString() << std::endl;
        ss << indent("+Elements:", 1) << std::endl;
        ss << indent(print(stmt->elements), 2);

        return ss.str();
    }

    template<>
    std::string print<TypeStmt *>(TypeStmt *stmt) {
        std::stringstream ss;

        ss << "TypeStmt" << std::endl;
        ss << indent("+Name: ", 1) + stmt->name->lexeme << std::endl;
        ss << indent("+Descriptor:", 1) << std::endl;
        ss << indent(stmt->typeDescriptor->toString(), 2) << std::endl;

        return ss.str();
    }

    template<>
    std::string print<IdentifierCallExpr *>(IdentifierCallExpr *stmt) {
        std::stringstream ss;

        ss << "IdentifierCallExpr" << std::endl;
        ss << indent("+Identifier: ", 1) + stmt->identifier->lexeme << std::endl;
        ss << indent("+Args:", 1) << std::endl;
        ss << indent(print(stmt->args), 2);

        return ss.str();
    }

    template<>
    std::string print<GetCallExpr *>(GetCallExpr *stmt) {
        std::stringstream ss;

        ss << "GetCallExpr" << std::endl;
        ss << indent("+Callee: ", 1) << std::endl;
        ss << indent(print(stmt->callee), 2);
        ss << indent("+Identifier: ", 1) + stmt->identifier->lexeme << std::endl;
        ss << indent("+Args:", 1) << std::endl;
        ss << indent(print(stmt->args), 2);

        return ss.str();
    }

    template<>
    std::string print<FunctionExpr *>(FunctionExpr *stmt) {
        std::stringstream ss;

        ss << "FunctionExpr" << std::endl;
        ss << indent(print(stmt->functionStmt), 2);

        return ss.str();
    }
}