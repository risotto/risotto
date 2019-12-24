//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_TOKEN_H
#define RISOTTOV2_TOKEN_H

extern "C" {
#include <lib/vm/value.h>
}
#include <string>
#include "../../../lib/better-enums/enum.h"

class Position {
public:
    int line;
    int column;

    Position(int line, int column);

    std::string toString();
};

BETTER_ENUM(TokenType, int,
// Single-character tokens.
            LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY, LEFT_SQUARED, RIGHT_SQUARED,
            COMMA, DOT, MINUS, PLUS, PERCENT, SEMICOLON, COLON, SLASH, STAR,

// One or two character tokens.
            QUESTION_MARK,
            BANG, BANG_EQUAL,
            EQUAL, EQUAL_EQUAL,
            GREATER, GREATER_EQUAL,
            LESS, LESS_EQUAL,
            COLON_EQUAL,
            PLUS_EQUAL,
            MINUS_EQUAL,
            SLASH_EQUAL,
            STAR_EQUAL,
            PLUS_PLUS,
            MINUS_MINUS,
            AMPERSAND,

// Literals.
            IDENTIFIER, STRING, INT, DOUBLE,

// Keywords
            AND, ELSE, FALSE, FUNC, FOR, IF, NIL, OR,
            RETURN, THIS, TRUE, WHILE, NEW, TYPE, STRUCT, OP,

            TOKEN_EOF
)

class Token {
public:
    TokenType type;
    ValueData value;
    std::string lexeme;
    Position position;

    Token(TokenType type,
          ValueData value,
          std::string lexeme,
          Position position
    );

    static Token* IdentifierFactory(const std::string& id);
};


#endif //RISOTTOV2_TOKEN_H
