//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_COMPILERERROR_H
#define RISOTTOV2_COMPILERERROR_H

#include <exception>
#include <string>
#include "TypesTable.h"

class CompilerError : public std::exception {
public:
    CompilerError(const std::string& message, Position position);

    std::string message;

    CompilerError(const std::string &message);

    [[nodiscard]] const char *what() const noexcept override;
};

class FunctionNotFoundError : public CompilerError {
public:
    FunctionNotFoundError(const std::string& name, const std::string& type, const std::vector<TypeReference *>& argsTypes, Token *hook);
    FunctionNotFoundError(Token *name, const std::string& type);
};

#endif //RISOTTOV2_COMPILERERROR_H
