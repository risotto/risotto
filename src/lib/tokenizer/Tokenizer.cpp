//
// Created by rvigee on 10/1/19.
//

#include <string>
#include <utility>
#include <map>
#include <sstream>
#include "Tokenizer.h"
#include "SyntaxError.h"

namespace literal_to_value_data {
    template<typename T>
    ValueData convert(T literal) {
        throw SyntaxError("Unhandled literal", new Position(0, 0));
    }

    template<>
    ValueData convert<int>(int literal) {
        return ValueData{._int = literal};
    }

    template<>
    ValueData convert<double>(double literal) {
        return ValueData{._double = literal};
    }

    template<>
    ValueData convert<const char *>(const char *literal) {
        return ValueData{._str = literal};
    }

    template<>
    ValueData convert<std::string>(std::string str) {
        char *cstr = new char[str.length() + 1];
        std::strcpy(cstr, str.c_str());

        return ValueData{._str = cstr};
    }
}

Tokenizer::Tokenizer(std::string src) : src(std::move(src)) {}

std::vector<Token *> Tokenizer::tokenize() {
    while (!isAtEnd()) {
        scan();
    }

    addToken(Token::Type::TOKEN_EOF);

    return tokens;
}

bool Tokenizer::isAtEnd() {
    return current >= src.length();
}

void Tokenizer::scan() {
    start = current;

    auto c = advance();
    switch (c) {
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace.
            break;
        case '\n':
            nextLine();
            break;
        case '"':
            lexString();
            break;
        case '(':
            addToken(Token::Type::LEFT_PAREN);
            break;
        case ')':
            addToken(Token::Type::RIGHT_PAREN);
            break;
        case '{':
            addToken(Token::Type::LEFT_CURLY);
            break;
        case '}':
            addToken(Token::Type::RIGHT_CURLY);
            break;
        case '[':
            addToken(Token::Type::LEFT_SQUARED);
            break;
        case ']':
            addToken(Token::Type::RIGHT_SQUARED);
            break;
        case ',':
            addToken(Token::Type::COMMA);
            break;
        case '.':
            addToken(Token::Type::DOT);
            break;
        case '-':
            if (match('=')) {
                addToken(Token::Type::MINUS_EQUAL);
            } else if (match('-')) {
                addToken(Token::Type::MINUS_MINUS);
            } else {
                addToken(Token::Type::MINUS);
            }
            break;
        case '+':
            if (match('=')) {
                addToken(Token::Type::PLUS_EQUAL);
            } else if (match('+')) {
                addToken(Token::Type::PLUS_PLUS);
            } else {
                addToken(Token::Type::PLUS);
            }
            break;
        case '%':
            addToken(Token::Type::PERCENT);
            break;
        case '&':
            addToken(match('&') ? Token::Type::AND : Token::Type::AMPERSAND);
            break;
        case '|':
            if (match('|')) {
                addToken(Token::Type::OR);
            }
            break;
        case ';':
            addToken(Token::Type::SEMICOLON);
            break;
        case ':':
            addToken(match('=') ? Token::Type::COLON_EQUAL : Token::Type::COLON);
            break;
        case '*':
            addToken(match('=') ? Token::Type::STAR_EQUAL : Token::Type::STAR);
            break;
        case '?':
            addToken(Token::Type::QUESTION_MARK);
            break;
        case '!':
            addToken(match('=') ? Token::Type::BANG_EQUAL : Token::Type::BANG);
            break;
        case '=':
            addToken(match('=') ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL);
            break;
        case '<':
            addToken(match('=') ? Token::Type::LESS_EQUAL : Token::Type::LESS);
            break;
        case '>':
            addToken(match('=') ? Token::Type::GREATER_EQUAL : Token::Type::GREATER);
            break;
        case '/':
            if (match('/')) {
                // A comment goes until the end of the line.
                while (!match('\n') && !isAtEnd()) advance();
            } else if (match('*')) {
                // Multiline comment
                while (!isAtEnd()) {
                    if (match('\n')) {
                        nextLine();
                    } else if (match('*')) {
                        if (match('/')) {
                            break;
                        }
                    } else {
                        advance();
                    }
                }
            } else if (match('=')) {
                addToken(Token::Type::SLASH_EQUAL);
            } else {
                addToken(Token::Type::SLASH);
            }
            break;

        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                std::stringstream ss;
                ss << "Unexpected character " << c;

                throw SyntaxError(ss.str(), new Position(line, column));
            }
    }
}

void Tokenizer::addToken(Token::Type type) {
    addToken(type, "");
}

template<typename T>
void Tokenizer::addToken(Token::Type type, T literal) {
    std::string lexeme = src.substr(start, current - start);
    tokens.push_back(
            new Token(type, literal_to_value_data::convert(literal), lexeme,
                      Position(line + 1, column - lexeme.size() + 1))
    );
}

bool Tokenizer::match(char expected) {
    if (isAtEnd()) return false;
    if (src.at(current) != expected) return false;

    current++;
    return true;
}

void Tokenizer::lexString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            nextLine();
        };
        advance();
    }

    // Unterminated string.
    if (isAtEnd()) {
        throw SyntaxError("Unterminated string", new Position(line, column));
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = src.substr(start + 1, current - 1 - (start + 1));
    addToken(Token::Type::STRING, value);
}

bool Tokenizer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

void Tokenizer::number() {
    bool isDecimal = false;
    while (isDigit(peek())) advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
        isDecimal = true;

        // Consume the "."
        advance();

        while (isDigit(peek())) advance();
    }

    auto s = src.substr(start, current - start);

    if (isDecimal) {
        addToken(Token::Type::DOUBLE, std::stod(s));
    } else {
        addToken(Token::Type::INT, std::stoi(s));
    }
}

char Tokenizer::peek() {
    return peekN(0);
}

char Tokenizer::peekNext() {
    return peekN(1);
}

char Tokenizer::peekN(int n) {
    if (current + n >= src.length()) {
        return '\0';
    }

    return src.at(current + n);
}

bool Tokenizer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

std::map<std::string, Token::Type> Tokenizer::keywords = {
        {"else",   Token::Type::ELSE},
        {"false",  Token::Type::FALSE},
        {"for",    Token::Type::FOR},
        {"func",   Token::Type::FUNC},
        {"op",     Token::Type::OP},
        {"if",     Token::Type::IF},
        {"nil",    Token::Type::NIL},
        {"return", Token::Type::RETURN},
        {"this",   Token::Type::THIS},
        {"new",    Token::Type::NEW},
        {"true",   Token::Type::TRUE},
        {"while",  Token::Type::WHILE},
};

void Tokenizer::identifier() {
    while (isAlphaNumeric(peek())) advance();

    auto text = src.substr(start, current - start);

    Token::Type type = Token::Type::_from_index(0); // Just has to be something, will be replaced immediately...

    if (keywords.find(text) == keywords.end()) {
        type = Token::Type::IDENTIFIER;
    } else {
        type = keywords.at(text);
    }

    addToken(type, text);
}

bool Tokenizer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}


char Tokenizer::advance() {
    auto c = src.at(current);
    current++;
    column++;

    return c;
}

void Tokenizer::nextLine() {
    line++;
    column = 0;
}
