//
// Created by rvigee on 10/1/19.
//

#include "Token.h"

#include <type_traits>
#include <sstream>

Position::Position(int line, int column): line(line), column(column) {

}

std::string Position::toString() {
    std::stringstream ss;
    ss << line << ":" << column;

    return ss.str();
}

Token::Token(Type type, TokenValue value, std::string lexeme, Position position): type(type), value(value), lexeme(std::move(lexeme)), position(position) {

}
