//
// Created by rvigee on 10/1/19.
//

#include "Token.h"

#include <type_traits>

Position::Position(int line, int column): line(line), column(column) {

}

Token::Token(TokenType type, TokenValue value, std::string lexeme, Position position): type(type), value(value), lexeme(std::move(lexeme)), position(position) {

}
