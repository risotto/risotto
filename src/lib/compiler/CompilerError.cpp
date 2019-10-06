//
// Created by rvigee on 10/4/19.
//

#include "CompilerError.h"

#include <utility>
#include <sstream>

CompilerError::CompilerError(const std::string &message) {
    this->message = message;
}

const char *CompilerError::what() const noexcept {
    return message.c_str();
}

std::string
format(const std::string &name, const std::string &type, const std::vector<TypeEntry *> *argsTypes, Token *hook) {
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

            ss << (*argsTypes)[i]->name;
        }

        ss << ")";

    }

    ss << " at " << hook->position.toString();

    return ss.str();
}

FunctionNotFoundError::FunctionNotFoundError(Token *name, const std::string &type) :
        CompilerError(format(name->lexeme, type, nullptr, name)) {

}

FunctionNotFoundError::FunctionNotFoundError(
        const std::string &name,
        const std::string &type,
        const std::vector<TypeEntry *> &argsTypes,
        Token *hook
) : CompilerError(format(name, type, &argsTypes, hook)) {

}
