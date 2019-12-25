//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_TOKEN_H
#define RISOTTOV2_TOKEN_H

extern "C" {
#include <lib/vm/value.h>
}
#include <string>
#include "TokenType.h"

class Position {
public:
    int line;
    int column;

    Position(int line, int column);

    std::string toString();
};


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
