//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_TOKEN_H
#define RISOTTOV2_TOKEN_H

#include <string>
#include <lib/vm/value.h>
#include "../../../lib/better-enums/enum.h"

class Position {
public:
    int line;
    int column;

    Position(int line, int column);

    std::string toString();
};

BETTER_ENUM(_token_type, int,
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
            IDENTIFIER, STRING, INT, DOUBLE,

// Keywords
            AND, ELSE, FALSE, FUNC, FOR, IF, NIL, OR,
            RETURN, THIS, TRUE, WHILE, NEW, TYPE, STRUCT,

            TOKEN_EOF
)

class Token {
public:
    using Type = _token_type;

    Type type;
    ValueData value;
    std::string lexeme;
    Position position;

    Token(Type
          type,
          ValueData value,
          std::string lexeme,
          Position position
    );
};


#endif //RISOTTOV2_TOKEN_H
