//
// Created by rvigee on 10/4/19.
//

#include "CompilerError.h"

#include <utility>
#include <sstream>

CompilerError::CompilerError(const std::string &message) {
    this->message = message;
}

CompilerError::CompilerError(const std::string& message, Position position) :
        CompilerError(message + " at " + position.toString()) {

}

const char *CompilerError::what() const noexcept {
    return message.c_str();
}

std::string
format(const std::string &name, const std::string &type, const std::vector<TypeReference> *argsTypes) {
    std::stringstream ss;
    ss << "Cannot find function matching ";

    if (!type.empty()) {
        ss << type << ".";
    }

    if (!name.empty()) {
        ss << name;
    }

    if (argsTypes != nullptr) {
        ss << "(";

        for (size_t i = 0; i < argsTypes->size(); ++i) {
            if (i != 0) {
                ss << ", ";
            }

            ss << (*argsTypes)[i].entry->name;
        }

        ss << ")";

    }

    return ss.str();
}

FunctionNotFoundError::FunctionNotFoundError(Token *name, const std::string &type) :
        CompilerError(format(name->lexeme, type, nullptr), name->position) {

}

FunctionNotFoundError::FunctionNotFoundError(
        const std::string &name,
        const std::string &type,
        const std::vector<TypeReference> &argsTypes,
        Token *hook
) : CompilerError(format(name, type, &argsTypes), hook->position) {

}
