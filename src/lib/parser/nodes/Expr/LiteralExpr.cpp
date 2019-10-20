//
// Created by rvigee on 10/2/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include <lib/compiler/CompilerError.h>
#include <lib/compiler/TypeDefinition.h>
#include "LiteralExpr.h"
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeReference.h"
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
    v.vtable = getReturnTypeDefinition(compiler)->vtable;

    auto constAddr = compiler->registerConst(v);

    bytes.push_back(new ByteResolver(OP_CONST, nullptr));
    bytes.push_back(new ByteResolver([constAddr](Compiler *c) { return constAddr; }, nullptr));

    return bytes;
}

std::string LiteralExpr::getReturnTypeName() {
    switch (value->type) {
        case TokenType::FALSE:
        case TokenType::TRUE:
            return "bool";
        case TokenType::NIL:
            throw CompilerError("Cannot get type of nil ");
        case TokenType::INT:
            return "int";
        case TokenType::DOUBLE:
            return "double";
        case TokenType::STRING:
            return "string";
        default:
            throw CompilerError("Unhandled type " + std::string(value->type._to_string()));
    }
}

TypeDefinition *LiteralExpr::getReturnTypeDefinition(Compiler *compiler) {
    return compiler->frame->findNamedType(getReturnTypeName());
}

ReturnTypes LiteralExpr::computeReturnType(Compiler *compiler) {
    return new NamedTypeReference(getReturnTypeName(), getReturnTypeDefinition(compiler));
}
