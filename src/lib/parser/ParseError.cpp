//
// Created by rvigee on 10/2/19.
//

#include <sstream>
#include "ParseError.h"

ParseError::ParseError(const std::string& message, Token *token): token(token) {
    std::stringstream ss;
    ss << message << " at " << position_string(token->position);

    this->message = ss.str();
}

const char *ParseError::what() const noexcept {
    return message.c_str();
}
