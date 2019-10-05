//
// Created by rvigee on 10/1/19.
//

#include <utility>
#include <iostream>
#include <lib/compiler/CompilerError.h>
#include "Parser.h"
#include "ParseError.h"
#include "lib/parser/nodes/Stmt/ExpressionStmt.h"
#include "lib/parser/nodes/Expr/SetExpr.h"
#include "lib/parser/nodes/Expr/LogicalExpr.h"
#include "lib/parser/nodes/Expr/BinaryExpr.h"
#include "lib/parser/nodes/Expr/UnaryExpr.h"
#include "lib/parser/nodes/Expr/CallExpr.h"
#include "lib/parser/nodes/Expr/LiteralExpr.h"
#include "lib/parser/nodes/Expr/VariableExpr.h"
#include "lib/parser/nodes/Expr/GroupingExpr.h"
#include "lib/parser/nodes/Expr.h"
#include "lib/parser/nodes/Stmt.h"
#include "lib/parser/nodes/Stmt/IfStmt.h"
#include "lib/parser/nodes/Stmt/ReturnStmt.h"
#include "lib/parser/nodes/ParameterDefinition.h"
#include "lib/parser/nodes/Stmt/FunctionStmt.h"
#include "lib/parser/nodes/Stmt/BlockStmt.h"

Parser::Parser(std::vector<Token *> tokens) : tokens(std::move(tokens)) {

}

Token *Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek()->type._value == Token::Type::TOKEN_EOF;
}

template<class ...Types>
bool Parser::match(Types... types) {
    for (Token::Type type : {types...}) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(Token::Type tokenType) {
    if (isAtEnd()) return false;

    return peek()->type == tokenType;
}

Token *Parser::peek() {
    return peek(0);
}

Token *Parser::peek(unsigned int n) {
    return tokens.at(current + n);
}

Token *Parser::previous() {
    return tokens.at(current - 1);
}

Token *Parser::consume(Token::Type type, const std::string &message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

ParseError Parser::error(Token *token, const std::string &message) {
    return ParseError(message, token);
}

std::vector<Stmt *> Parser::program() {
    std::vector<Stmt *> nodes;

    while (!isAtEnd()) {
        auto s = declaration();

        if (s == nullptr) {
            auto token = peek();
            throw CompilerError("Unexpected token " + token->lexeme + " at " + token->position.toString());
        }

        nodes.push_back(s);
    }

    return nodes;
}

Stmt *Parser::declaration() {
    if (match(Token::Type::TYPE)) {
        throw error(previous(), "TODO TYPE");
    }

    if (match(Token::Type::FUNC)) {
        return function();
    }

    if (match(Token::Type::STRUCT)) {
        throw error(previous(), "TODO STRUCT");
    }

    return statement();
}

Stmt *Parser::function() {
    Token *name = consume(Token::Type::IDENTIFIER, "Expect function name.");

    consume(Token::Type::LEFT_PAREN, "Expect '(' after function name.");

    std::vector<ParameterDefinition> parameters = enumeration<ParameterDefinition>([this]() {
        auto name = consume(Token::Type::IDENTIFIER, "Expect parameter name.");
        auto type = consume(Token::Type::IDENTIFIER, "Expect return type.");;

        return ParameterDefinition(type, name);
    }, Token::Type::RIGHT_PAREN);

    consume(Token::Type::RIGHT_PAREN, "Expect ')' after parameters.");

    Token *returnType = consume(Token::Type::IDENTIFIER, "Expect return type.");

    auto closeBlock = consume(Token::Type::LEFT_CURLY, "Expect '{' before function body.");
    std::vector<Stmt *> body = block();

    return new FunctionStmt(name, returnType, parameters, body, closeBlock);
}

std::vector<Stmt *> Parser::block() {
    std::vector<Stmt *> statements;

    while (!check(Token::Type::RIGHT_CURLY) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(Token::Type::RIGHT_CURLY, "Expect '}' after block.");
    return statements;
}

Stmt *Parser::statement() {
//    if (match(FOR)) return forStatement();
    if (match(Token::Type::IF)) return ifStatement();
    if (match(Token::Type::RETURN)) return returnStatement();
    if (match(Token::Type::LEFT_CURLY)) return new BlockStmt(block());
//    if (match(WHILE)) return whileStatement();

    return expressionStatement();
}

Stmt *Parser::ifStatement() {
    Expr *condition = expression();

    Stmt *thenBranch = statement();

    return new IfStmt(condition, thenBranch);
}


Stmt *Parser::returnStatement() {
    Token *keyword = previous();
    Expr *value = expression();

    return new ReturnStmt(keyword, value);
}

Stmt *Parser::expressionStatement() {
    Expr *expr = expression();

    if (expr == nullptr) {
        return nullptr;
    }

    return new ExpressionStmt(expr);
}

Expr *Parser::expression() {
    return assignment();
}

Expr *Parser::assignment() {
    Expr *expr = logicalOr();

    if (match(
            Token::Type::EQUAL,
            Token::Type::PLUS_EQUAL,
            Token::Type::MINUS_EQUAL,
            Token::Type::STAR_EQUAL,
            Token::Type::SLASH_EQUAL
    )) {
        Token *op = previous();
        Expr *value = assignment();

        return new SetExpr(expr, op, value);
    }

    return expr;
}

Expr *Parser::logicalOr() {
    Expr *expr = logicalAnd();

    while (match(Token::Type::OR)) {
        Token *op = previous();
        Expr *right = logicalAnd();
        expr = new LogicalExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::logicalAnd() {
    Expr *expr = equality();

    while (match(Token::Type::AND)) {
        Token *op = previous();
        Expr *right = equality();
        expr = new LogicalExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::equality() {
    Expr *expr = comparison();

    while (match(Token::Type::EQUAL_EQUAL, Token::Type::BANG_EQUAL)) {
        Token *op = previous();
        Expr *right = comparison();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::comparison() {
    Expr *expr = addition();

    while (match(Token::Type::EQUAL_EQUAL, Token::Type::BANG_EQUAL)) {
        Token *op = previous();
        Expr *right = addition();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::addition() {
    Expr *expr = multiplication();

    while (match(Token::Type::PLUS, Token::Type::SLASH)) {
        Token *op = previous();
        Expr *right = multiplication();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::multiplication() {
    Expr *expr = unary();

    while (match(Token::Type::MINUS, Token::Type::PLUS)) {
        Token *op = previous();
        Expr *right = unary();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::unary() {
    if (match(Token::Type::BANG, Token::Type::MINUS, Token::Type::MINUS_MINUS, Token::Type::PLUS_PLUS)) {
        Token *op = previous();
        Expr *right = unary();
        return new UnaryExpr(op, right);
    }

    return call();
}

Expr *Parser::call() {
    Expr *expr = primary();

    while (true) {
        auto identifier = previous();
        if (match(Token::Type::LEFT_PAREN)) {
            auto args = arguments();

            Token *rParen = consume(Token::Type::RIGHT_PAREN, "Expect ')' after parameters.");

            expr = new CallExpr(identifier, rParen, args);
        } else {
            break;
        }
    }

    return expr;
}

Expr *Parser::primary() {
    if (match(
            Token::Type::FALSE,
            Token::Type::TRUE,
            Token::Type::NIL,
            Token::Type::INT,
            Token::Type::DOUBLE,
            Token::Type::STRING
    )) {
        return new LiteralExpr(previous());
    }

    if (match(Token::Type::IDENTIFIER)) {
        return new VariableExpr(previous());
    }

    if (match(Token::Type::LEFT_PAREN)) return group();

    return nullptr;
}

Expr *Parser::group() {
    auto lParen = previous();
    Expr *expr = expression();

    auto rParen = consume(Token::Type::RIGHT_PAREN, "Expect ')' after expression.");

    return new GroupingExpr(expr, lParen, rParen);
}

std::vector<Expr *> Parser::arguments() {
    return enumeration<Expr *>([this]() { return this->expression(); }, Token::Type::RIGHT_PAREN);
}

template<typename T>
std::vector<T> Parser::enumeration(std::function<T()> of, Token::Type end) {
    std::vector<T> elements;
    if (!check(end)) {
        do {
            elements.push_back(of());
        } while (match(Token::Type::COMMA));
    }

    return elements;
}
