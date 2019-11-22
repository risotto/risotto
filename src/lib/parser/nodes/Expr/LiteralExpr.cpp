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
            throw CompilerError("Unhandled type " + std::string(value->type._to_string()));
    }
}

std::vector<ByteResolver *> LiteralExpr::compile(Compiler *compiler) {
    auto bytes = std::vector<ByteResolver *>();

    switch (value->type) {
        case TokenType::NIL:
            bytes.push_back(new ByteResolver(OP_NIL, nullptr));
            return bytes;
        case TokenType::TRUE:
            bytes.push_back(new ByteResolver(OP_TRUE, nullptr));
            return bytes;
        case TokenType::FALSE:
            bytes.push_back(new ByteResolver(OP_FALSE, nullptr));
            return bytes;
    }

    auto v = literalToValue(value);
    auto constAddr = compiler->registerConst(v);

    bytes.push_back(new ByteResolver(OP_CONST, nullptr));
    bytes.push_back(new ByteResolver([constAddr](Compiler *c) { return constAddr; }, nullptr));

    return bytes;
}

TypeDescriptor *getTypeDescriptor(Compiler *compiler, std::string name) {
    auto typeDef = compiler->frame->findNamedType(name);

    return new IdentifierTypeDescriptor(name, typeDef->getTypeDefinition());
}

ReturnTypes LiteralExpr::computeReturnType(Compiler *compiler) {
    switch (value->type) {
        case TokenType::FALSE:
        case TokenType::TRUE:
            return getTypeDescriptor(compiler, "bool");
        case TokenType::NIL:
            throw CompilerError("Cannot get type of nil ");
        case TokenType::INT:
            return getTypeDescriptor(compiler, "int");
        case TokenType::DOUBLE:
            return getTypeDescriptor(compiler, "double");
        case TokenType::STRING:
            return getTypeDescriptor(compiler, "string");
        default:
            throw CompilerError("Unhandled type " + std::string(value->type._to_string()));
    }}

void LiteralExpr::symbolize(Compiler *compiler) {
    // noop
}
