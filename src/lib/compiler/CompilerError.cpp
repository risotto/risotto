//
// Created by rvigee on 10/4/19.
//

#include "CompilerError.h"

CompilerError::CompilerError(const std::string& message) {
    this->message = message;
}

const char *CompilerError::what() const noexcept {
    return message.c_str();
}
