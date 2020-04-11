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
#include <lib/parser/nodes/Expr/GetCallExpr.h>
#include <lib/parser/nodes/Expr/IdentifierCallExpr.h>
#include <lib/parser/nodes/Expr/NewCallExpr.h>
#include <lib/parser/nodes/Expr/SubscriptExpr.h>
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

Shorthand::Shorthand(TokenType op, std::string str) : op(op), str(std::move(str)) {}

Parser::Parser(std::vector<const Token *> tokens) : tokens(std::move(tokens)) {

}

const Token *Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek()->type == TokenType::TOKEN_EOF;
}

template<class ...Types>
bool Parser::match(Types... types) {
    for (TokenType type : {types...}) {
        if (check(type)) {
            advance();
            return true;
        }
    }

    return false;
}

bool Parser::check(TokenType tokenType) {
    return check(tokenType, 0);
}

bool Parser::check(TokenType tokenType, int n) {
    if (isAtEnd()) return false;

    return peek(n)->type == tokenType;
}

const Token *Parser::peek() {
    return peek(0);
}

const Token *Parser::peek(unsigned int n) {
    return tokens.at(current + n);
}

const Token *Parser::previous() {
    return tokens.at(current - 1);
}

const Token *Parser::consume(TokenType type, const std::string &message) {
    if (check(type)) return advance();

    throw error(peek(), message);
}

ParseError Parser::error(const Token *token, const std::string &message) {
    return ParseError(message, token);
}

TypeDescriptor *Parser::typeDesc() {
    if (match(TokenType::LEFT_RIGHT_SQUARED)) {

        return new ArrayTypeDescriptor(typeDesc());
    }

    if (match(TokenType::STRUCT)) {
        consume(TokenType::LEFT_CURLY, "Expect '{'");

        auto fields = enumeration<StructTypeDescriptor::Field>([this]() {
            auto name = consume(TokenType::IDENTIFIER, "Expect field name");
            auto type = typeDesc();

            return StructTypeDescriptor::Field(name, type);
        }, TokenType::SEMICOLON, TokenType::RIGHT_CURLY);

        consume(TokenType::RIGHT_CURLY, "Expect '}'");

        return new StructTypeDescriptor(fields);
    }

    if (match(TokenType::INTERFACE)) {
        consume(TokenType::LEFT_CURLY, "Expect '{'");

        auto functions = enumeration<FunctionEntry *>([this]() {
            return functionSignature<FunctionEntry *>(
                    TokenType::FUNC,
                    false,
                    true,
                    [](FUNCTION_SIGNATURE_FACTORY_ARGS) {
                        auto desc = new FunctionTypeDescriptor(true, std::move(parameters), std::move(returnTypes));

                        return new DeclarationFunctionEntry(name->lexeme, desc);
                    }
            );
        }, TokenType::SEMICOLON, TokenType::RIGHT_CURLY);

        consume(TokenType::RIGHT_CURLY, "Expect '}'");

        return new InterfaceTypeDescriptor(functions);
    }

    if (match(TokenType::FUNC)) {
        return functionSignature<FunctionTypeDescriptor *>(
                previous()->type,
                false,
                false,
                [](FUNCTION_SIGNATURE_FACTORY_ARGS) {
                    return new FunctionTypeDescriptor(receiver != nullptr, std::move(parameters),
                                                      std::move(returnTypes));
                }
        );
    }

    if (match(TokenType::IDENTIFIER)) {
        auto name = previous();
        return new IdentifierTypeDescriptor(name);
    }

    return nullptr;
}

std::vector<Stmt *> Parser::program() {
    std::vector<Stmt *> nodes;

    while (!isAtEnd()) {
        auto s = declaration();

        if (s == nullptr) {
            auto token = peek();
            throw CompilerError("Unexpected token " + token->lexeme, token->position);
        }

        nodes.push_back(s);
    }

    return nodes;
}

Stmt *Parser::declaration() {
    if (match(TokenType::TYPE)) {
        return type();
    }

    if (match(TokenType::FUNC, TokenType::OP, TokenType::NEW)) {
        auto type = previous();
        auto isConstructor = type->type == TokenType::NEW;

        auto isNamed = !isConstructor;

        return function<FunctionStmt *>(true, isNamed, [isConstructor](FUNCTION_FACTORY_ARGS) {
            if (isConstructor) {
                if (receiver == nullptr) {
                    throw CompilerError("Constructors must have receiver");
                }

                if (returnTypes.size() != 1 || !returnTypes[0]->isSame(receiver->type)) {
                    throw CompilerError("Constructors must return " + receiver->type->toString());
                }
            }

            return new FunctionStmt(
                    typeDecl,
                    receiver,
                    name,
                    returnTypes,
                    parameters,
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
    auto identifier = consume(TokenType::IDENTIFIER, "Expect type name.");

    auto desc = typeDesc();

    return new TypeStmt(identifier, desc);
}

Stmt *Parser::varDecl() {
    auto c = current;
    if (check(TokenType::IDENTIFIER)) {
        auto identifiers = enumeration<std::pair<const Token *, TypeDescriptor *>>([this]() {
            auto identifier = consume(TokenType::IDENTIFIER, "Expect identifier.");

            auto type = typeDesc();

            return std::make_pair<const Token *, TypeDescriptor *>(
                    reinterpret_cast<const Token *&&>(identifier),
                    reinterpret_cast<TypeDescriptor *&&>(type)
            );
        }, TokenType::COMMA, TokenType::COLON_EQUAL);

        if (match(TokenType::COLON_EQUAL)) {
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
T
Parser::functionSignature(TokenType type, bool canHaveReceiver, bool canBeNamed, const FunctionSignatureFactory<T> &f) {
    ParameterDefinition *receiver = nullptr;
    if (canHaveReceiver && match(TokenType::LEFT_PAREN)) {
        receiver = parameter();

        consume(TokenType::RIGHT_PAREN, "Expect ')' after receiver declaration.");
    }

    const Token *name = nullptr;
    if (canBeNamed) {
        switch (type) {
            case TokenType::FUNC:
                name = consume(TokenType::IDENTIFIER, "Expect function name.");
                break;
            case TokenType::OP:
                name = advance(); // anything
                break;
            default:
                throw ParseError("Unexpected function type: " + std::string(wise_enum::to_string(type)), peek(0));
        }
    }

    auto endStr = std::string("");
    if (canBeNamed) {
        endStr = " after function name";
    } else if (canHaveReceiver) {
        endStr = " after receiver";
    }

    consume(TokenType::LEFT_PAREN, "Expect '('" + endStr + ".");

    auto parameters = enumeration<ParameterDefinition *>([this]() {
        return parameter();
    }, TokenType::RIGHT_PAREN);

    consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

    std::vector<TypeDescriptor *> returnTypes;
    if (match(TokenType::LEFT_PAREN)) {
        returnTypes = enumeration<TypeDescriptor *>([this]() {
            return typeDesc();
        }, TokenType::RIGHT_PAREN);

        consume(TokenType::RIGHT_PAREN, "Expect ')' after return types.");
    } else {
        auto returnType = typeDesc();

        if (returnType != nullptr) {
            returnTypes = std::vector<TypeDescriptor *>({returnType});
        }
    }

    return f(type, receiver, name, returnTypes, parameters);
}

template<typename T>
T Parser::function(bool canHaveReceiver, bool isNamed, const FunctionFactory<T> &f) {
    auto typeDecl = previous();

    return functionSignature<T>(
            typeDecl->type,
            canHaveReceiver,
            isNamed,
            [this, f, typeDecl](FUNCTION_SIGNATURE_FACTORY_ARGS) {
                auto closeBlock = consume(TokenType::LEFT_CURLY, "Expect '{'.");

                std::vector<Stmt *> body = block();

                return f(typeDecl, type, receiver, name, returnTypes, parameters, body, closeBlock);
            }
    );
}

ParameterDefinition *Parser::parameter() {
    auto name = consume(TokenType::IDENTIFIER, "Expect parameter name.");

    auto asReference = false;
    if (match(TokenType::AMPERSAND)) {
        asReference = true;
    }

    auto type = typeDesc();

    return new ParameterDefinition(name, type, asReference);
}

std::vector<Stmt *> Parser::block() {
    std::vector<Stmt *> statements;

    while (!check(TokenType::RIGHT_CURLY) && !isAtEnd()) {
        auto stmt = declaration();

        if (stmt == nullptr) {
            auto c = tokens[current];
            throw ParseError("Unexpected token " + c->lexeme, c);
        }

        statements.push_back(stmt);
    }

    consume(TokenType::RIGHT_CURLY, "Expect '}' after block.");
    return statements;
}

Stmt *Parser::statement() {
    if (match(TokenType::FOR)) return forStatement();
    if (match(TokenType::IF)) return ifStatement(true);
    if (match(TokenType::RETURN)) return returnStatement();
    if (match(TokenType::LEFT_CURLY)) return new BlockStmt(block());
    if (match(TokenType::WHILE)) return whileStatement();

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
    if (match(TokenType::SEMICOLON)) {
        initializer = nullptr;
    } else {
        auto varDeclStmt = varDecl();

        if (varDeclStmt != nullptr) {
            initializer = varDeclStmt;
        } else {
            initializer = expressionStatement();
        }
    }

    consume(TokenType::SEMICOLON, "Expect ';' after loop initializer.");

    Expr *condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

    Expr *increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
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
    while (canHaveElse && match(TokenType::ELSE)) {
        if (match(TokenType::IF)) {
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

        if (!match(TokenType::COMMA)) {
            break;
        }
    }

    auto keyword = previous();

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
    if (match(TokenType::FUNC)) {
        auto functionStmt = function<FunctionStmt *>(false, false, [](FUNCTION_FACTORY_ARGS) {
            return new FunctionStmt(
                    typeDecl,
                    receiver,
                    name,
                    returnTypes,
                    parameters,
                    std::move(body),
                    closeBlock
            );
        });
        functionStmt->autoRegister = false;

        return new FunctionExpr(functionStmt);
    }

    if (check(TokenType::LEFT_RIGHT_SQUARED)) {
        auto type = typeDesc();

        consume(TokenType::LEFT_CURLY, "Expect '{'");

        auto elements = enumeration<Expr *>([this]() {
            return expression();
        }, TokenType::RIGHT_CURLY);

        consume(TokenType::RIGHT_CURLY, "Expect '}'");

        return new ArrayExpr(type, elements);
    }

    if (match(TokenType::NEW)) {
        auto identifier = consume(TokenType::IDENTIFIER, "Expect identifier");

        consume(TokenType::LEFT_PAREN, "Expect '('");

        auto args = arguments(TokenType::RIGHT_PAREN);

        auto rParen = consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

        return new NewCallExpr(identifier, rParen, args);
    }

    return assignment();
}

const auto shorthands = std::map<TokenType, Shorthand>(
        {
                // Maths
                {TokenType::PLUS_EQUAL, Shorthand(TokenType::PLUS, "+")},
                {TokenType::MINUS_EQUAL, Shorthand(TokenType::MINUS, "-")},
                {TokenType::STAR_EQUAL, Shorthand(TokenType::STAR, "*")},
                {TokenType::SLASH_EQUAL, Shorthand(TokenType::SLASH, "/")},

                // Bitwise
                {TokenType::AMPERSAND_EQUAL, Shorthand(TokenType::AMPERSAND, "&")},
                {TokenType::PIPE_EQUAL, Shorthand(TokenType::PIPE, "|")},
                {TokenType::CARET_EQUAL, Shorthand(TokenType::CARET, "^")},
                {TokenType::LEFT_LEFT_EQUAL, Shorthand(TokenType::LEFT_LEFT, "<<")},
                {TokenType::RIGHT_RIGHT_EQUAL, Shorthand(TokenType::RIGHT_RIGHT, ">>")},
        }
);

Expr *Parser::assignment() {
    auto expr = logicalOr();

    if (match(TokenType::EQUAL)) {
        auto op = previous();
        auto value = expression();

        return new SetExpr(expr, op, value);
    }

    if (match(
            TokenType::PLUS_EQUAL,
            TokenType::MINUS_EQUAL,
            TokenType::STAR_EQUAL,
            TokenType::SLASH_EQUAL,
            TokenType::AMPERSAND_EQUAL,
            TokenType::PIPE_EQUAL,
            TokenType::CARET_EQUAL,
            TokenType::LEFT_LEFT_EQUAL,
            TokenType::RIGHT_RIGHT_EQUAL
    )) {
        auto op = previous();
        auto value = expression();

        auto s = shorthands.at(op->type);

        return new SetExpr(
                expr,
                new Token(TokenType::EQUAL, {}, "=", op->position),
                new BinaryExpr(
                        expr,
                        new Token(s.op, {}, s.str, op->position),
                        value
                )
        );
    }

    return expr;
}

Expr *Parser::logicalOr() {
    auto expr = logicalAnd();

    while (match(TokenType::OR)) {
        auto op = previous();
        auto right = logicalAnd();
        expr = new LogicalExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::logicalAnd() {
    auto expr = bit_or();

    while (match(TokenType::AND)) {
        auto op = previous();
        auto right = bit_or();
        expr = new LogicalExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::bit_or() {
    auto expr = bit_xor();

    while (match(TokenType::PIPE)) {
        auto op = previous();
        auto right = bit_xor();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::bit_xor() {
    auto expr = bit_and();

    while (match(TokenType::CARET)) {
        auto op = previous();
        auto right = bit_and();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::bit_and() {
    auto expr = equality();

    while (match(TokenType::AMPERSAND)) {
        auto op = previous();
        auto right = equality();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::equality() {
    auto expr = comparison();

    while (match(TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL)) {
        auto op = previous();
        auto right = comparison();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::comparison() {
    auto expr = bitshifting();

    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
        auto op = previous();
        auto right = addition();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::bitshifting() {
    auto expr = addition();

    while (match(TokenType::LEFT_LEFT, TokenType::RIGHT_RIGHT)) {
        auto op = previous();
        auto right = bitshifting();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::addition() {
    auto expr = multiplication();

    while (match(TokenType::MINUS, TokenType::PLUS)) {
        auto op = previous();
        auto right = multiplication();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::multiplication() {
    auto expr = unary();

    while (match(TokenType::SLASH, TokenType::STAR, TokenType::PERCENT)) {
        auto op = previous();
        auto right = unary();
        expr = new BinaryExpr(expr, op, right);
    }

    return expr;
}

Expr *Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS, TokenType::TILDE, TokenType::MINUS_MINUS, TokenType::PLUS_PLUS)) {
        auto op = previous();
        auto *right = unary();
        return new UnaryExpr(op, right);
    }

    return call();
}

Expr *Parser::call() {
    auto expr = primary();

    while (true) {
        if (match(TokenType::LEFT_PAREN)) {
            auto args = arguments(TokenType::RIGHT_PAREN);

            auto rParen = consume(TokenType::RIGHT_PAREN, "Expect ')' after parameters.");

            if (auto identifierExpr = dynamic_cast<IdentifierExpr *>(expr)) {
                expr = new IdentifierCallExpr(identifierExpr->name, rParen, args);
            } else if (auto getExpr = dynamic_cast<GetExpr *>(expr)) {
                expr = new GetCallExpr(getExpr->callee, rParen, getExpr->identifier, rParen, args);
            } else {
                throw error(peek(), "Invalid call");
            }
        } else if (match(TokenType::DOT)) {
            auto identifier = consume(TokenType::IDENTIFIER, "Expect identifier.");

            expr = new GetExpr(expr, identifier);
        } else if (match(TokenType::LEFT_SQUARED)) {
            auto args = arguments(TokenType::RIGHT_SQUARED);

            auto rParen = consume(TokenType::RIGHT_SQUARED, "Expect ']' after parameters.");

            expr = new SubscriptExpr(expr, rParen, args);
        } else {
            break;
        }
    }

    return expr;
}

Expr *Parser::primary() {
    if (match(
            TokenType::FALSE,
            TokenType::TRUE,
            TokenType::NIL,
            TokenType::INT,
            TokenType::DOUBLE,
            TokenType::STRING
    )) {
        return new LiteralExpr(previous());
    }

    if (match(TokenType::IDENTIFIER)) {
        return new IdentifierExpr(previous());
    }

    if (match(TokenType::LEFT_PAREN)) return group();

    return nullptr;
}

Expr *Parser::group() {
    auto lParen = previous();
    auto expr = expression();

    auto rParen = consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");

    return new GroupingExpr(expr, lParen, rParen);
}

std::vector<Expr *> Parser::arguments(TokenType end) {
    return enumeration<Expr *>([this]() { return this->expression(); }, end);
}

template<typename T>
std::vector<T> Parser::enumeration(std::function<T()> of, TokenType end) {
    return enumeration(of, TokenType::COMMA, end);
}

template<typename T>
std::vector<T> Parser::enumeration(std::function<T()> of, TokenType separator, TokenType end) {
    std::vector<T> elements;

    do {
        if (check(end)) {
            break;
        }

        elements.push_back(of());
    } while (match(separator));

    return elements;
}
