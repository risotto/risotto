//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_TOKEN_H
#define RISOTTOV2_TOKEN_H

extern "C" {
#include <lib/vm/value.h>
#include <lib/vm/position.h>
}
#include <string>
#include "TokenType.h"

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
