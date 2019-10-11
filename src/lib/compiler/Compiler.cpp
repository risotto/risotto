//
// Created by rvigee on 10/3/19.
//

#include "Compiler.h"

#include <utility>

extern "C" {
#include <lib/vm/native_functions.h>
}

#define NATIVE_BINARY_DECLARATION_NAMED(target, op, param, return, functionName) \
    target##Entry->addOperator( \
        "self", \
        true, \
        new NativeFunctionEntry( \
            #op, \
            {FunctionEntryParameter("right", new NamedTypeReference(param##Entry))}, \
            {new NamedTypeReference(return##Entry)}, \
            functionName \
        ) \
    );

#define NATIVE_BINARY_OPERATOR_DECLARATION(target, op, param, return, opName) \
NATIVE_BINARY_DECLARATION_NAMED(target, op, param, return, binary_##target##_##opName##_##param)

#define NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(target, param, return) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, +, param, return, add) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, -, param, return, sub) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, *, param, return, mul) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, /, param, return, div) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, <, param, bool, lower) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, >, param, bool, greater) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, <=, param, bool, lower_equal) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, >=, param, bool, greater_equal) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, ==, param, bool, eq) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, !=, param, bool, neq) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, +=, param, return, add_equal) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, -=, param, return, sub_equal) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, *=, param, return, mul_equal) \
NATIVE_BINARY_OPERATOR_DECLARATION(target, /=, param, return, div_equal)

#define NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(type) \
NATIVE_BINARY_OPERATOR_DECLARATION(string, +, type, string, add) \
NATIVE_BINARY_OPERATOR_DECLARATION(type, +, string, string, add) \
NATIVE_BINARY_OPERATOR_DECLARATION(string, +=, type, string, add_equal)

#define NATIVE_UNARY_OPERATOR_DECLARATION(target, op, return, functionName) \
    target##Entry->addPrefix( \
        "self", \
        true, \
        new NativeFunctionEntry( \
            #op, \
            {}, \
            {new NamedTypeReference(return##Entry)}, \
            functionName \
        ) \
    );

#define NATIVE_UNARY_PREFIX_OPERATOR_DECLARATION(target, op, return, functionName) \
NATIVE_UNARY_OPERATOR_DECLARATION(target, op, return, unary_prefix_##target##_##functionName)

#define NATIVE_UNARY_OPERATOR_MATH_DECLARATIONS(target, return) \
NATIVE_UNARY_PREFIX_OPERATOR_DECLARATION(target, -, return, negate) \
NATIVE_UNARY_PREFIX_OPERATOR_DECLARATION(target, ++, return, increment) \
NATIVE_UNARY_PREFIX_OPERATOR_DECLARATION(target, --, return, decrement) \

#define NATIVE_PRINT(type) \
    frame->functions.add( \
        new NativeFunctionEntry( \
            "println", \
            {FunctionEntryParameter("e",  new NamedTypeReference(type##Entry))}, \
            {}, \
            println_##type \
        ) \
    );

Compiler::Compiler(std::vector<Stmt *> stmts) : stmts(std::move(stmts)) {
    frame = new Frame();

    initChunk(&chunk);

    auto intEntry = frame->types.add("int");
    auto doubleEntry = frame->types.add("double");
    auto boolEntry = frame->types.add("bool");
    auto stringEntry = frame->types.add("string");

    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(int, int, int)
    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(int, double, double)
    NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(int)
    NATIVE_BINARY_OPERATOR_DECLARATION(int, %, int, int, mod)

    NATIVE_UNARY_OPERATOR_MATH_DECLARATIONS(int, int)

    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(double, double, double)
    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(double, int, double)
    NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(double)

    NATIVE_UNARY_OPERATOR_MATH_DECLARATIONS(double, double)

    NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(bool)

    NATIVE_BINARY_OPERATOR_DECLARATION(string, +, string, string, add)
    NATIVE_BINARY_OPERATOR_DECLARATION(string, +=, string, string, add_equal)

    boolEntry->addPrefix(
            "self",
            true,
            new NativeFunctionEntry(
                    "!",
                    {},
                    {new NamedTypeReference(boolEntry)},
                    unary_prefix_bool_invert
            )
    );

    NATIVE_PRINT(int)
    NATIVE_PRINT(double)
    NATIVE_PRINT(bool)
    NATIVE_PRINT(string)
}

Chunk Compiler::compile() {
    for (auto stmt: stmts) {
        auto stmtBytes = stmt->compile(this);
        bytes.insert(bytes.end(), stmtBytes.begin(), stmtBytes.end());
    }

    bytes.push_back(new ByteResolver(OP_END, nullptr));

    for (auto b: bytes) {
        b->finalize(this);

        // TODO: column
        int l = -1;
        if (b->position != nullptr) {
            l = b->position->line;
        }

        writeChunk(&chunk, b->byte, l);

        delete b;
    }

    return chunk;
}

OP_T Compiler::registerConst(Value v) {
    return addConstant(&chunk, v);
}

OP_T Compiler::getAddr(ByteResolver *byte) {
    for (unsigned long i = 0; i < bytes.size(); ++i) {
        auto c = bytes.at(i);

        if (c == byte) {
            return static_cast<int>(i);
        }
    }

    throw std::logic_error("Byte is not part of program");
}
