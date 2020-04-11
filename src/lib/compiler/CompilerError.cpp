//
// Created by rvigee on 10/4/19.
//

#include "CompilerError.h"

#include <utility>
#include <sstream>
#include <lib/parser/nodes/TypeDescriptor.h>

CompilerError::CompilerError(const std::string &message) {
    this->message = message;
}

CompilerError::CompilerError(const std::string &message, Position position) :
        CompilerError(message + " at " + position_string(position)) {

}

const char *CompilerError::what() const noexcept {
    return message.c_str();
}

std::string replace_str(std::string str, const std::string &from, const std::string &to) {
    while (str.find(from) != std::string::npos) {
        str.replace(str.find(from), from.length(), to);
    }

    return str;
}

std::string
format(const std::string &tpl, const std::vector<TypeDescriptor *> *argsTypes) {
    std::stringstream argsss;

    if (argsTypes != nullptr) {
        for (size_t i = 0; i < argsTypes->size(); ++i) {
            if (i != 0) {
                argsss << ", ";
            }

            argsss << (*argsTypes)[i]->toString();
        }
    }

    auto argsStr = argsss.str();

    auto str = replace_str(tpl, "{{args}}", argsStr);

    return "Undefined function: " + str;
}

FunctionNotFoundError::FunctionNotFoundError(
        const std::string &tpl,
        const std::vector<TypeDescriptor *> &argsTypes,
        const Token *hook
) : CompilerError(format(tpl, &argsTypes), hook->position) {

}
