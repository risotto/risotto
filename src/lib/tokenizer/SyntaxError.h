//
// Created by rvigee on 10/1/19.
//

#ifndef RISOTTOV2_SYNTAXERROR_H
#define RISOTTOV2_SYNTAXERROR_H

#include <string>
#include "Token.h"

class SyntaxError : public std::exception {
public:
    std::string message;
    Position *position;

    SyntaxError(const std::string &message, Position *position);

    [[nodiscard]] const char *what() const noexcept override;
};


#endif //RISOTTOV2_SYNTAXERROR_H
