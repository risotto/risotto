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

std::string format(const std::string& name, std::vector<TypeEntry *> argsTypes) {
    std::stringstream ss;
    ss << "Cannot find function matching ";
    ss << name;
    ss << "(";

    for (size_t i = 0; i < argsTypes.size(); ++i) {
        if (i != 0) {
            ss << ", ";
        }

        ss << argsTypes[i]->name;
    }

    ss << ")";

    return ss.str();
}

FunctionNotFoundError::FunctionNotFoundError(const std::string& name, std::vector<TypeEntry *> argsTypes) :
        CompilerError(format(name, std::move(argsTypes))) {

}
