//
// Created by rvigee on 10/1/19.
//

#include "SyntaxError.h"
#include <sstream>

SyntaxError::SyntaxError(const std::string &message, Position position) : position(position) {
    std::stringstream ss;
    ss << message << " at " << position_string(position);

    this->message = ss.str();
}

const char *SyntaxError::what() const noexcept {
    return message.c_str();
}
