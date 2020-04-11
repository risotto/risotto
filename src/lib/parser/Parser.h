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
    PToken name, \
    std::vector<TypeDescriptor *> returnTypes, \
    std::vector<ParameterDefinition *> parameters

template<typename T>
using FunctionSignatureFactory = std::function<T(FUNCTION_SIGNATURE_FACTORY_ARGS)>;

#define FUNCTION_FACTORY_ARGS \
    PToken typeDecl, \
    FUNCTION_SIGNATURE_FACTORY_ARGS, \
    std::vector<Stmt *> body, \
    PToken closeBlock

template<typename T>
using FunctionFactory = std::function<T(FUNCTION_FACTORY_ARGS)>;

class Shorthand {
public:
    TokenType op;
    std::string str;

    Shorthand(TokenType op, std::string str);
};

class Parser {
public:
    static std::vector<Stmt *> Parse(std::vector<Token> *tokens);

private:
    std::vector<PToken> tokens;
    unsigned int current = 0;

    explicit Parser(std::vector<PToken> tokens);

    PToken peek();

    PToken peek(unsigned int n);

    PToken previous();

    template<class... Types>
    bool match(Types... types);

    bool isAtEnd();

    bool check(TokenType tokenType);

    bool check(TokenType tokenType, int n);

    std::vector<Stmt *> program();

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

    Expr *bit_or();

    Expr *bit_xor();

    Expr *bit_and();

    Expr *equality();

    Expr *comparison();

    Expr *bitshifting();

    Expr *addition();

    Expr *multiplication();

    Expr *unary();

    PToken advance();

    Expr *call();

    Expr *primary();

    Expr *group();

    std::vector<Expr *> arguments(TokenType end);

    template<typename T>
    std::vector<T> enumeration(std::function<T()> of, TokenType separator, TokenType end);

    template<typename T>
    std::vector<T> enumeration(std::function<T()> of, TokenType end);

    PToken consume(TokenType type, const std::string &message);

    ParseError error(PToken token, const std::string &message);

    std::vector<Stmt *> block();

    template<typename T>
    T function(bool canHaveReceiver, bool isNamed, const FunctionFactory<T> &f);

    template<typename T>
    T functionSignature(TokenType type, bool canHaveReceiver, bool canBeNamed, const FunctionSignatureFactory<T> &f);

    ParameterDefinition *parameter();

    Stmt *varDecl();

    TypeDescriptor *typeDesc();

    Stmt *type();
};


#endif //RISOTTOV2_PARSER_H
