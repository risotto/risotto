//
// Created by rvigee on 12/25/19.
//

#ifndef RISOTTOV2_TOKENTYPE_H
#define RISOTTOV2_TOKENTYPE_H

#include "../lib/wise_enum/wise_enum.h"

WISE_ENUM(TokenType,
// Single-character tokens.
          LEFT_PAREN, RIGHT_PAREN, LEFT_CURLY, RIGHT_CURLY, LEFT_SQUARED, RIGHT_SQUARED,
          COMMA, DOT, MINUS, PLUS, PERCENT, SEMICOLON, COLON, SLASH, STAR, PIPE, CARET, TILDE,

// One or two character tokens.
          QUESTION_MARK,
          BANG, BANG_EQUAL,
          EQUAL, EQUAL_EQUAL,
          GREATER, GREATER_EQUAL,
          LESS, LESS_EQUAL,
          COLON_EQUAL,
          PLUS_PLUS,
          MINUS_MINUS,
          AMPERSAND,
          LEFT_RIGHT_SQUARED,
          LEFT_LEFT,
          RIGHT_RIGHT,

          PLUS_EQUAL,
          MINUS_EQUAL,
          SLASH_EQUAL,
          STAR_EQUAL,

          AMPERSAND_EQUAL,
          PIPE_EQUAL,
          CARET_EQUAL,
          LEFT_LEFT_EQUAL,
          RIGHT_RIGHT_EQUAL,

// Literals.
          IDENTIFIER, STRING, INT, DOUBLE,

// Keywords
          AND, ELSE, FALSE, FUNC, FOR, IF, NIL, OR,
          RETURN, THIS, TRUE, WHILE, NEW, TYPE, STRUCT, OP, INTERFACE,

          TOKEN_EOF
);

#endif //RISOTTOV2_TOKENTYPE_H
