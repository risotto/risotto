//
// Created by rvigee on 10/1/19.
//

#include "Token.h"

#include <type_traits>
#include <sstream>

Token::Token(TokenType type, ValueData value, std::string lexeme, Position position) : type(type), value(value),
                                                                                       lexeme(std::move(lexeme)),
                                                                                       position(position) {

}

Token *Token::IdentifierFactory(const std::string& id) {
   return  new Token(TokenType::IDENTIFIER, ValueData {._str = id.c_str()}, id, {});
}
