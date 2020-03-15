//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include <lib/compiler/CompilerError.h>
#include <lib/parser/nodes/TypeDescriptor.h>
#include "LiteralExpr.h"
#include "lib/compiler/Compiler.h"
#include "lib/compiler/ReturnTypes.h"
#include "../lib/wise_enum/wise_enum.h"

LiteralExpr::LiteralExpr(Token *value) : value(value) {

}

Value literalToValue(Token *value) {
    switch (value->type) {
        case TokenType::INT:
            return i2v(value->value._int);
        case TokenType::DOUBLE:
            return d2v(value->value._double);
        case TokenType::STRING:
            return s2v(value->value._str);
        default:
            throw CompilerError("Unhandled type " + std::string(wise_enum::to_string(value->type)));
    }
}

std::vector<ByteResolver *> LiteralExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    switch (value->type) {
        case TokenType::NIL:
            bytes.push_back(new ByteResolver(OP_NIL, value->position));
            return bytes;
        case TokenType::TRUE:
            bytes.push_back(new ByteResolver(OP_TRUE, value->position));
            return bytes;
        case TokenType::FALSE:
            bytes.push_back(new ByteResolver(OP_FALSE, value->position));
            return bytes;
        default:
            auto v = literalToValue(value);
            auto constAddr = compiler->registerConst(v);

            bytes.push_back(new ByteResolver(OP_CONST, value->position));
            bytes.push_back(new ByteResolver([constAddr](Compiler *c) { return constAddr; }));

            return bytes;
    }
}

TypeDescriptor *getTypeDescriptor(Compiler *compiler, const std::string &name) {
    auto typeDesc = compiler->frame->findNamedType(name);

    return new IdentifierTypeDescriptor(name, typeDesc->getTypeDefinition());
}

ReturnTypes LiteralExpr::computeReturnType(Compiler *compiler) {
    switch (value->type) {
        case TokenType::FALSE:
        case TokenType::TRUE:
            return getTypeDescriptor(compiler, "bool");
        case TokenType::NIL:
            return new NilTypeDescriptor(value);
        case TokenType::INT:
            return getTypeDescriptor(compiler, "int");
        case TokenType::DOUBLE:
            return getTypeDescriptor(compiler, "double");
        case TokenType::STRING:
            return getTypeDescriptor(compiler, "string");
        default:
            throw CompilerError("Unhandled type " + std::string(wise_enum::to_string(value->type)));
    }
}

void LiteralExpr::symbolize(Compiler *compiler) {
    // noop
}
