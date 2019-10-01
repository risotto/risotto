//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_TOKEN_H
#define RISOTTOV2_TOKEN_H

#include <string>

union TokenValue {
    int _int;
    const char *_str;
    bool _bool;
};

enum TokenType {
    // Single-character tokens.
    LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY, LEFT_SQUARED, RIGHT_SQUARED,
    COMMA, DOT, MINUS, PLUS, PERCENT, SEMICOLON, COLON, SLASH, STAR,

    // One or two character tokens.
    QUESTION_MARK,
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,
    PLUS_EQUAL,
    MINUS_EQUAL,
    SLASH_EQUAL,
    STAR_EQUAL,
    PLUS_PLUS,
    MINUS_MINUS,
    AMPERSAND,

    // Literals.
    VAR, IDENTIFIER, STRING, INT, DOUBLE,

    // Keywords
    AND, CLASS, ELSE, FALSE, FUNC, FOR, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, WHILE, NEW, INIT, STATIC, OPERATOR, PREFIX, POSTFIX,

    TOKEN_EOF
};

class Position {
public:
    int line;
    int column;

    Position(int line, int column);
};

class Token {
public:
    TokenType type;
    TokenValue value;
    std::string lexeme;
    Position position;

    Token(TokenType type, TokenValue value, std::string lexeme, Position position);
};


#endif //RISOTTOV2_TOKEN_H
