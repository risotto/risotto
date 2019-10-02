//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_PARSEERROR_H
#define RISOTTOV2_PARSEERROR_H


#include <exception>
#include <lib/tokenizer/Token.h>

class ParseError: public std::exception {
public:
    std::string message;
    Token *token;

    ParseError(const std::string& message, Token *token);

    [[nodiscard]] const char *what() const noexcept override;
};

#endif //RISOTTOV2_PARSEERROR_H
