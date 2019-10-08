//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_PARSER_H
#define RISOTTOV2_PARSER_H

#include <map>
#include <lib/tokenizer/Token.h>
#include <vector>
#include <lib/parser/nodes/Node.h>
#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/nodes/Expr.h>
#include "ParseError.h"
#include "lib/parser/nodes/TypeDescriptor.h"

class Parser {
private:
    std::vector<Token *> tokens;
    unsigned int current = 0;

public:
    explicit Parser(std::vector<Token *> tokens);

    std::vector<Stmt *> program();

private:
    Token *peek();

    Token *peek(unsigned int n);

    Token *previous();

    template<class... Types>
    bool match(Types... types);

    bool isAtEnd();

    bool check(Token::Type tokenType);

    bool check(Token::Type tokenType, int n);

    Stmt *declaration();

    Stmt *statement();

    Stmt *whileStatement();

    Stmt *forStatement();

    Stmt *ifStatement(bool canHaveElse);

    Stmt *returnStatement();

    Stmt *expressionStatement();

    Expr *expression();

    Expr *assignment();

    Expr *logicalOr();

    Expr *logicalAnd();

    Expr *equality();

    Expr *comparison();

    Expr *addition();

    Expr *multiplication();

    Expr *unary();

    Token *advance();

    Expr *call();

    Expr *primary();

    Expr *group();

    std::vector<Expr *> arguments();

    template<typename T>
    std::vector<T> enumeration(std::function<T()> of, Token::Type end);

    Token *consume(Token::Type type, const std::string &message);

    ParseError error(Token *token, const std::string &message);

    std::vector<Stmt *> block();

    Stmt *function(bool isNamed);

    ParameterDefinition *parameter();

    Stmt *varDecl();

    TypeDescriptor *typeRef();

    InterfaceTypeDescriptor *interface();
};


#endif //RISOTTOV2_PARSER_H
