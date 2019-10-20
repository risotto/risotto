//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_PARSER_H
#define RISOTTOV2_PARSER_H

#include <map>
#include <vector>
#include "lib/parser/nodes/ParameterDefinition.h"
#include "ParseError.h"
#include "lib/parser/nodes/Stmt.h"
#include "lib/parser/nodes/Expr.h"
#include "lib/parser/nodes/TypeDescriptor.h"

#define FUNCTION_SIGNATURE_FACTORY_ARGS \
    TokenType type, \
    ParameterDefinition *receiver, \
    Token *name, \
    std::vector<TypeDescriptor *> returnTypes, \
    std::vector<ParameterDefinition> parameters

template<typename T>
using FunctionSignatureFactory = std::function<T(FUNCTION_SIGNATURE_FACTORY_ARGS)>;

#define FUNCTION_FACTORY_ARGS \
    Token *typeDecl, \
    FUNCTION_SIGNATURE_FACTORY_ARGS, \
    std::vector<Stmt *> body, \
    Token *closeBlock

template<typename T>
using FunctionFactory = std::function<T(FUNCTION_FACTORY_ARGS)>;

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

    bool check(TokenType tokenType);

    bool check(TokenType tokenType, int n);

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
    std::vector<T> enumeration(std::function<T()> of, TokenType separator, TokenType end);

    template<typename T>
    std::vector<T> enumeration(std::function<T()> of, TokenType end);

    Token *consume(TokenType type, const std::string &message);

    ParseError error(Token *token, const std::string &message);

    std::vector<Stmt *> block();

    template<typename T>
    T function(bool canHaveReceiver, bool isNamed, const FunctionFactory<T>& f);

    template<typename T>
    T functionSignature(TokenType type, bool canHaveReceiver, bool canBeNamed, const FunctionSignatureFactory<T>& f);

    ParameterDefinition *parameter();

    Stmt *varDecl();

    TypeDescriptor *typeDesc();

    Stmt *type();
};


#endif //RISOTTOV2_PARSER_H
