//
// Created by rvigee on 10/1/19.
//

#include <utility>
#include <lib/compiler/CompilerError.h>
#include <lib/parser/nodes/Expr/GetExpr.h>
#include <lib/parser/nodes/Expr/VarDeclStmt.h>
#include <lib/parser/nodes/Stmt/ForStmt.h>
#include <lib/parser/nodes/Expr/FunctionExpr.h>
#include <lib/parser/nodes/Expr/ArrayExpr.h>
#include <lib/parser/nodes/Stmt/TypeStmt.h>
#include <lib/parser/nodes/Expr/NewExpr.h>
#include "Parser.h"
#include "ParseError.h"
#include "lib/parser/nodes/Stmt/ExpressionStmt.h"
#include "lib/parser/nodes/Expr/SetExpr.h"
#include "lib/parser/nodes/Expr/LogicalExpr.h"
#include "lib/parser/nodes/Expr/BinaryExpr.h"
#include "lib/parser/nodes/Expr/UnaryExpr.h"
#include "lib/parser/nodes/Expr/CallExpr.h"
#include "lib/parser/nodes/Expr/LiteralExpr.h"
#include "lib/parser/nodes/Expr/IdentifierExpr.h"
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
    return check(tokenType, 0);
}

bool Parser::check(Token::Type tokenType, int n) {
    if (isAtEnd()) return false;

    return peek(n)->type == tokenType;
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

TypeDescriptor *Parser::typeDesc() {
    if (match(Token::Type::LEFT_SQUARED)) {
        consume(Token::Type::RIGHT_SQUARED, "Expect ']'");

        return new ArrayTypeDescriptor(typeDesc());
    }

    if (match(Token::Type::STRUCT)) {
        consume(Token::Type::LEFT_CURLY, "Expect '{'");

        auto fields = enumeration<StructTypeDescriptor::Field>([this]() {
            auto name = consume(Token::Type::IDENTIFIER, "Expect field name");
            auto type = typeDesc();

            return StructTypeDescriptor::Field(name, type);
        }, Token::Type::SEMICOLON, Token::Type::RIGHT_CURLY);

        consume(Token::Type::RIGHT_CURLY, "Expect '}'");

        return new StructTypeDescriptor(fields);
    }

    auto name = consume(Token::Type::IDENTIFIER, "Expect type");

    return new IdentifierTypeDescriptor(name);
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
        return type();
    }

    if (match(Token::Type::FUNC, Token::Type::OP, Token::Type::NEW)) {
        auto type = previous();
        auto isConstructor = type->type._value == Token::Type::NEW;

        auto isNamed = !isConstructor;

        return function<FunctionStmt *>(true, isNamed, [isConstructor](FUNCTION_NODE_ARGS) {
            if (isConstructor) {
                if (returnTypes.size() != 1) {
                    throw CompilerError("Constructors must return the object");
                }

                if (receiver == nullptr) {
                    throw CompilerError("Constructors must have receiver");
                }

                if (returnTypes[0]->toString() != receiver->type->toString()) {
                    throw CompilerError("Constructors must return the object");
                }
            }

            return new FunctionStmt(
                    type,
                    receiver,
                    name,
                    std::move(returnTypes),
                    std::move(parameters),
                    std::move(body),
                    closeBlock
            );
        });
    }

    auto varDeclStmt = varDecl();
    if (varDeclStmt) {
        return varDeclStmt;
    }

    return statement();
}

Stmt *Parser::type() {
    auto identifier = consume(Token::Type::IDENTIFIER, "Expect type name.");

    auto desc = typeDesc();

    return new TypeStmt(identifier, desc);
}

Stmt *Parser::varDecl() {
    auto c = current;
    if (match(Token::Type::IDENTIFIER)) {
        auto identifiers = std::vector<Token *>({previous()});
        while (match(Token::Type::COMMA)) {
            identifiers.push_back(consume(Token::Type::IDENTIFIER, "Expect identifier."));
        }

        if (match(Token::Type::COLON_EQUAL)) {
            auto op = previous();

            auto value = expression();

            return new VarDeclStmt(identifiers, op, value);
        } else {
            // Reset and keep going
            current = c;
        }
    }

    return nullptr;
}

template<typename T>
T Parser::function(bool canHaveReceiver, bool isNamed, const FunctionNodeType<T> &f) {
    auto type = previous();

    ParameterDefinition *receiver = nullptr;
    if (canHaveReceiver && match(Token::Type::LEFT_PAREN)) {
        receiver = parameter();

        consume(Token::Type::RIGHT_PAREN, "Expect ')' after receiver declaration.");
    }

    Token *name = nullptr;
    if (isNamed) {
        switch (type->type) {
            case Token::Type::FUNC:
                name = consume(Token::Type::IDENTIFIER, "Expect function name.");
                break;
            case Token::Type::OP:
                name = advance(); // anything
                break;
            default:
                throw ParseError("Unexpected function type", type);
        }
    }

    consume(Token::Type::LEFT_PAREN, "Expect '(' after function name.");

    std::vector<ParameterDefinition> parameters = enumeration<ParameterDefinition>([this]() {
        return *parameter();
    }, Token::Type::RIGHT_PAREN);

    consume(Token::Type::RIGHT_PAREN, "Expect ')' after parameters.");

    auto returnTypes = enumeration<TypeDescriptor *>([this]() {
        return typeDesc();
    }, Token::Type::LEFT_CURLY);

    auto closeBlock = consume(Token::Type::LEFT_CURLY, "Expect '}'.");

    std::vector<Stmt *> body = block();

    return f(type, receiver, name, returnTypes, parameters, body, closeBlock);
}

ParameterDefinition *Parser::parameter() {
    auto name = consume(Token::Type::IDENTIFIER, "Expect parameter name.");

    auto asReference = false;
    if (match(Token::Type::AMPERSAND)) {
        asReference = true;
    }

    auto type = typeDesc();

    return new ParameterDefinition(name, type, asReference);
}

std::vector<Stmt *> Parser::block() {
    std::vector<Stmt *> statements;

    while (!check(Token::Type::RIGHT_CURLY) && !isAtEnd()) {
        auto stmt = declaration();

        if (stmt == nullptr) {
            auto c = tokens[current];
            throw ParseError("Unexpected token " + c->lexeme, c);
        }

        statements.push_back(stmt);
    }

    consume(Token::Type::RIGHT_CURLY, "Expect '}' after block.");
    return statements;
}

Stmt *Parser::statement() {
    if (match(Token::Type::FOR)) return forStatement();
    if (match(Token::Type::IF)) return ifStatement(true);
    if (match(Token::Type::RETURN)) return returnStatement();
    if (match(Token::Type::LEFT_CURLY)) return new BlockStmt(block());
    if (match(Token::Type::WHILE)) return whileStatement();

    return expressionStatement();
}

Stmt *Parser::whileStatement() {
    Expr *condition = expression();

    Stmt *bodyStmt = statement();

    BlockStmt *body;
    if (dynamic_cast<BlockStmt *>(bodyStmt)) {
        body = dynamic_cast<BlockStmt *>(bodyStmt);
    } else {
        body = new BlockStmt({bodyStmt});
    }

    return new WhileStmt(condition, body);
}

Stmt *Parser::forStatement() {
    Stmt *initializer;
    if (match(Token::Type::SEMICOLON)) {
        initializer = nullptr;
    } else {
        auto varDeclStmt = varDecl();

        if (varDeclStmt != nullptr) {
            initializer = varDeclStmt;
        } else {
            initializer = expressionStatement();
        }
    }

    consume(Token::Type::SEMICOLON, "Expect ';' after loop initializer.");

    Expr *condition = nullptr;
    if (!check(Token::Type::SEMICOLON)) {
        condition = expression();
    }
    consume(Token::Type::SEMICOLON, "Expect ';' after loop condition.");

    Expr *increment = nullptr;
    if (!check(Token::Type::RIGHT_PAREN)) {
        increment = expression();
    }

    Stmt *bodyStmt = statement();

    BlockStmt *body;
    if (dynamic_cast<BlockStmt *>(bodyStmt)) {
        body = dynamic_cast<BlockStmt *>(bodyStmt);
    } else {
        body = new BlockStmt({bodyStmt});
    }

    if (increment != nullptr) {
        body->stmts.push_back(new ExpressionStmt(increment));
    }

    return new ForStmt(initializer, new WhileStmt(condition, body));
}

Stmt *Parser::ifStatement(bool canHaveElse) {
    Expr *condition = expression();

    Stmt *thenBranch = statement();
    auto elseifs = std::vector<IfStmt *>();
    Stmt *elseBranch = nullptr;
    while (canHaveElse && match(Token::Type::ELSE)) {
        if (match(Token::Type::IF)) {
            elseifs.push_back((IfStmt *) ifStatement(false));
        } else {
            elseBranch = statement();
            break;
        }
    }

    return new IfStmt(condition, thenBranch, elseifs, elseBranch);
}

Stmt *Parser::returnStatement() {
    auto values = std::vector<Expr *>();

    while (!isAtEnd()) {
        auto value = expression();

        if (value == nullptr) {
            break;
        }

        values.push_back(value);

        if (!match(Token::Type::COMMA)) {
            break;
        }
    }

    Token *keyword = previous();

    return new ReturnStmt(keyword, values);
}

Stmt *Parser::expressionStatement() {
    Expr *expr = expression();

    if (expr == nullptr) {
        return nullptr;
    }

    return new ExpressionStmt(expr);
}

Expr *Parser::expression() {
    if (match(Token::Type::FUNC)) {
        auto functionStmt = function<FunctionStmt *>(false, false, [](FUNCTION_NODE_ARGS) {
            return new FunctionStmt(
                    type,
                    receiver,
                    name,
                    std::move(returnTypes),
                    std::move(parameters),
                    std::move(body),
                    closeBlock
            );
        });
        functionStmt->autoRegister = false;

        return new FunctionExpr(functionStmt);
    }

    if (check(Token::Type::LEFT_SQUARED)) {
        auto type = typeDesc();

        consume(Token::Type::LEFT_CURLY, "Expect '{'");

        auto elements = enumeration<Expr *>([this]() {
            return expression();
        }, Token::Type::RIGHT_CURLY);

        consume(Token::Type::RIGHT_CURLY, "Expect '}'");

        return new ArrayExpr(type, elements);
    }

    if (match(Token::Type::NEW)) {
        auto identifier = consume(Token::Type::IDENTIFIER, "Expect identifier");
        auto identifierDesc = new IdentifierTypeDescriptor(identifier);

        consume(Token::Type::LEFT_PAREN, "Expect '('");

        auto args = arguments();

        Token *rParen = consume(Token::Type::RIGHT_PAREN, "Expect ')' after parameters.");

        return new CallExpr(new NewExpr(identifierDesc), rParen, args);
    }

    return assignment();
}

Expr *Parser::assignment() {
    Expr *expr = logicalOr();

    if (match(Token::Type::EQUAL)) {
        Token *op = previous();
        Expr *value = expression();

        return new SetExpr(expr, op, value);
    }

    if (match(
            Token::Type::PLUS_EQUAL,
            Token::Type::MINUS_EQUAL,
            Token::Type::STAR_EQUAL,
            Token::Type::SLASH_EQUAL
    )) {
        Token *op = previous();
        Expr *value = expression();

        return new BinaryExpr(expr, op, value);
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

    while (match(Token::Type::GREATER, Token::Type::GREATER_EQUAL, Token::Type::LESS, Token::Type::LESS_EQUAL)) {
        Token *op = previous();
        Expr *right = addition();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::addition() {
    Expr *expr = multiplication();

    while (match(Token::Type::MINUS, Token::Type::PLUS)) {
        Token *op = previous();
        Expr *right = multiplication();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::multiplication() {
    Expr *expr = unary();

    while (match(Token::Type::SLASH, Token::Type::STAR, Token::Type::PERCENT)) {
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
    auto expr = primary();

    while (true) {
        if (match(Token::Type::LEFT_PAREN)) {
            auto args = arguments();

            Token *rParen = consume(Token::Type::RIGHT_PAREN, "Expect ')' after parameters.");

            expr = new CallExpr(expr, rParen, args);
        } else if (match(Token::Type::DOT)) {
            auto identifier = consume(Token::Type::IDENTIFIER, "Expect identifier.");

            expr = new GetExpr(expr, identifier);
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
        return new IdentifierExpr(previous());
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
    return enumeration(of, Token::Type::COMMA, end);
}

template<typename T>
std::vector<T> Parser::enumeration(std::function<T()> of, Token::Type separator, Token::Type end) {
    std::vector<T> elements;

    do {
        if (check(end)) {
            break;
        }

        elements.push_back(of());
    } while (match(separator));

    return elements;
}
