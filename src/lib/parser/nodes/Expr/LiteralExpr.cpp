//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include <lib/compiler/CompilerError.h>
#include "LiteralExpr.h"
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeReference.h"

LiteralExpr::LiteralExpr(Token *value) : value(value) {

}

Value literalToValue(Token *value) {
    switch (value->type) {
        case Token::Type::INT:
            return i2v(value->value._int);
        case Token::Type::DOUBLE:
            return d2v(value->value._double);
        case Token::Type::STRING:
            return s2v(value->value._str);
        default:
            throw CompilerError("Unhandled type " + std::string(value->type._to_string()));
    }
}

std::vector<ByteResolver *> LiteralExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    switch (value->type) {
        case Token::Type::NIL:
            bytes.push_back(new ByteResolver(OP_NIL, nullptr));
            return bytes;
        case Token::Type::TRUE:
            bytes.push_back(new ByteResolver(OP_TRUE, nullptr));
            return bytes;
        case Token::Type::FALSE:
            bytes.push_back(new ByteResolver(OP_FALSE, nullptr));
            return bytes;
    }

    auto v = literalToValue(value);
    auto constAddr = compiler->registerConst(v);

    bytes.push_back(new ByteResolver(OP_CONST, nullptr));
    bytes.push_back(new ByteResolver([constAddr](Compiler *c) { return constAddr; }, nullptr));

    return bytes;
}

TypeReferences LiteralExpr::computeReturnType(Compiler *compiler) {
    switch (value->type) {
        case Token::Type::FALSE:
        case Token::Type::TRUE:
            return compiler->frame->findType("bool");
        case Token::Type::NIL:
            throw CompilerError("Cannot get type of nil ");
        case Token::Type::INT:
            return compiler->frame->findType("int");
        case Token::Type::DOUBLE:
            return compiler->frame->findType("double");
        case Token::Type::STRING:
            return compiler->frame->findType("string");
        default:
            throw CompilerError("Unhandled type " + std::string(value->type._to_string()));
    }
}
