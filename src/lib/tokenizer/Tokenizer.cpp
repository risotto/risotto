//
// Created by rvigee on 10/1/19.
//

#include <string>
#include <utility>
#include <map>
#include <sstream>
#include <lib/compiler/ByteResolver.h>
#include "Tokenizer.h"
#include "SyntaxError.h"

namespace literal_to_value_data {
    template<typename T>
    ValueData convert(T literal) {
        throw SyntaxError("Unhandled literal", TODO_POSITION);
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
        strcpy(cstr, str.c_str());

        return ValueData{._str = cstr};
    }
}

Tokenizer::Tokenizer(std::string src) : src(std::move(src)) {}

std::vector<Token> Tokenizer::tokenize() {
    while (!isAtEnd()) {
        scan();
    }

    addToken(TokenType::TOKEN_EOF);

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
            addToken(TokenType::LEFT_PAREN);
            break;
        case ')':
            addToken(TokenType::RIGHT_PAREN);
            break;
        case '{':
            addToken(TokenType::LEFT_CURLY);
            break;
        case '}':
            addToken(TokenType::RIGHT_CURLY);
            break;
        case '[':
            addToken(match(']') ? TokenType::LEFT_RIGHT_SQUARED : TokenType::LEFT_SQUARED);
            break;
        case ']':
            addToken(TokenType::RIGHT_SQUARED);
            break;
        case ',':
            addToken(TokenType::COMMA);
            break;
        case '.':
            addToken(TokenType::DOT);
            break;
        case '-':
            if (match('=')) {
                addToken(TokenType::MINUS_EQUAL);
            } else if (match('-')) {
                addToken(TokenType::MINUS_MINUS);
            } else {
                addToken(TokenType::MINUS);
            }
            break;
        case '+':
            if (match('=')) {
                addToken(TokenType::PLUS_EQUAL);
            } else if (match('+')) {
                addToken(TokenType::PLUS_PLUS);
            } else {
                addToken(TokenType::PLUS);
            }
            break;
        case '%':
            addToken(TokenType::PERCENT);
            break;
        case '&':
            if (match('&')) {
                addToken(TokenType::AND);
            } else if (match('=')) {
                addToken(TokenType::AMPERSAND_EQUAL);
            } else {
                addToken(TokenType::AMPERSAND);
            }
            break;
        case '|':
            if (match('|')) {
                addToken(TokenType::OR);
            } else if (match('=')) {
                addToken(TokenType::PIPE_EQUAL);
            } else {
                addToken(TokenType::PIPE);
            }
            break;
        case '^':
            if (match('=')) {
                addToken(TokenType::CARET_EQUAL);
            } else {
                addToken(TokenType::CARET);
            }
            break;
        case '~':
            addToken(TokenType::TILDE);
            break;
        case ';':
            addToken(TokenType::SEMICOLON);
            break;
        case ':':
            addToken(match('=') ? TokenType::COLON_EQUAL : TokenType::COLON);
            break;
        case '*':
            addToken(match('=') ? TokenType::STAR_EQUAL : TokenType::STAR);
            break;
        case '?':
            addToken(TokenType::QUESTION_MARK);
            break;
        case '!':
            addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            if (match('<')) {
                if (match('=')) {
                    addToken(TokenType::LEFT_LEFT_EQUAL);
                } else {
                    addToken(TokenType::LEFT_LEFT);
                }
            } else if (match('=')) {
                addToken(TokenType::LESS_EQUAL);
            } else {
                addToken(TokenType::LESS);
            }
            break;
        case '>':
            if (match('>')) {
                if (match('=')) {
                    addToken(TokenType::RIGHT_RIGHT_EQUAL);
                } else {
                    addToken(TokenType::RIGHT_RIGHT);
                }
            } else if (match('=')) {
                addToken(TokenType::GREATER_EQUAL);
            } else {
                addToken(TokenType::GREATER);
            }
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
                addToken(TokenType::SLASH_EQUAL);
            } else {
                addToken(TokenType::SLASH);
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

                throw SyntaxError(ss.str(), {.line = line, .column = column});
            }
    }
}

void Tokenizer::addToken(TokenType type) {
    addToken(type, "");
}

template<typename T>
void Tokenizer::addToken(TokenType type, T literal) {
    std::string lexeme = src.substr(start, current - start);
    tokens.push_back(
            Token(
                    type,
                    literal_to_value_data::convert(literal),
                    lexeme,
                    {.line = line + 1, .column = static_cast<int>(column - lexeme.size() + 1)}
            )
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
        }
        advance();
    }

    // Unterminated string.
    if (isAtEnd()) {
        throw SyntaxError("Unterminated string", {.line = line, .column = column});
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = src.substr(start + 1, current - 1 - (start + 1));
    addToken(TokenType::STRING, value);
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
        addToken(TokenType::DOUBLE, std::stod(s));
    } else {
        addToken(TokenType::INT, std::stoi(s));
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

std::map<std::string, TokenType> Tokenizer::keywords = {
        {"else",      TokenType::ELSE},
        {"false",     TokenType::FALSE},
        {"for",       TokenType::FOR},
        {"func",      TokenType::FUNC},
        {"op",        TokenType::OP},
        {"if",        TokenType::IF},
        {"nil",       TokenType::NIL},
        {"return",    TokenType::RETURN},
        {"this",      TokenType::THIS},
        {"new",       TokenType::NEW},
        {"true",      TokenType::TRUE},
        {"while",     TokenType::WHILE},
        {"type",      TokenType::TYPE},
        {"struct",    TokenType::STRUCT},
        {"interface", TokenType::INTERFACE},
};

void Tokenizer::identifier() {
    while (isAlphaNumeric(peek())) advance();

    auto text = src.substr(start, current - start);

    TokenType type;

    if (keywords.find(text) == keywords.end()) {
        type = TokenType::IDENTIFIER;
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

std::vector<Token> Tokenizer::Tokenize(std::string str) {
    return Tokenizer(std::move(str)).tokenize();
}
