//
// Created by rvigee on 10/3/19.
//

#include "Compiler.h"
#include "TypeDefinition.h"

#include <utility>
#include <sstream>
#include <lib/parser/nodes/TypeDescriptor.h>

extern "C" {
#include <lib/vm/native_functions.h>
}

#define TYPE_ENTRY(type) type##Entry
#define ENTRY_DEF(entry) entry->getTypeDefinition()
#define TYPE_DESC(type) new IdentifierTypeDescriptor(#type, ENTRY_DEF(TYPE_ENTRY(type)))
#define SELF_RECEIVER(name, type) new ParameterDefinition(name, TYPE_DESC(type), true)

#define FUNCTION_ENTRY_VAR(target, functionName) function_entry__##functionName
#define REGISTER_FUNCTION(target, functionName) typesManager->registerFunction(ENTRY_DEF(TYPE_ENTRY(target)), FUNCTION_ENTRY_VAR(target, functionName));
#define REGISTER_FUNCTION2(target, functionEntry) typesManager->registerFunction(ENTRY_DEF(TYPE_ENTRY(target)), functionEntry);

#define NATIVE_BINARY_DECLARATION_NAMED(target, op, param, return, functionName) \
    auto FUNCTION_ENTRY_VAR(target, functionName) = ENTRY_DEF(TYPE_ENTRY(target))->addOperator( \
        SELF_RECEIVER("self", target), \
        new NativeFunctionEntry( \
            #op, \
            new FunctionTypeDescriptor( \
                true, \
                {new ParameterDefinition("right", TYPE_DESC(param), true)}, \
                {TYPE_DESC(return)} \
            ), \
            functionName \
        ) \
    ); \
    REGISTER_FUNCTION(target, functionName);

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
NATIVE_BINARY_OPERATOR_DECLARATION(target, !=, param, bool, neq)

#define NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(type) \
NATIVE_BINARY_OPERATOR_DECLARATION(string, +, type, string, add) \
NATIVE_BINARY_OPERATOR_DECLARATION(type, +, string, string, add)

#define NATIVE_UNARY_OPERATOR_DECLARATION(target, op, return, functionName) \
    auto FUNCTION_ENTRY_VAR(target, functionName) = ENTRY_DEF(TYPE_ENTRY(target))->addPrefix( \
        SELF_RECEIVER("left", target), \
        new NativeFunctionEntry( \
           #op, \
            new FunctionTypeDescriptor( \
                true, \
                {}, \
                {TYPE_DESC(return)} \
            ), \
            functionName \
        ) \
    ); \
    REGISTER_FUNCTION(target, functionName);

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
            new FunctionTypeDescriptor( \
                false, \
                {new ParameterDefinition("e", TYPE_DESC(type), true)}, \
                {} \
            ), \
            println_##type \
        ) \
    );

#define _BYTES_OPERATOR_DECLARATION(target, op, params, return, generator) \
    REGISTER_FUNCTION2(target, ENTRY_DEF(TYPE_ENTRY(target))->addOperator( \
        SELF_RECEIVER("i", target), \
        new BytesFunctionEntry( \
            #op, \
            new FunctionTypeDescriptor( \
                true, \
                params, \
                return \
            ), \
            generator \
        ) \
    )); \

#define BYTES_OPERATOR_DECLARATION(target, op, param, return, generator) \
    _BYTES_OPERATOR_DECLARATION(target, op, {new ParameterDefinition("right", TYPE_DESC(param), true)}, {TYPE_DESC(return)}, generator)

#define BYTES_OPERATOR_MATH_DECLARATIONS(t, prefix) \
    BYTES_OPERATOR_DECLARATION(t, ==, t, bool, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_EQ, nullptr)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, !=, t, bool, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_NEQ, nullptr)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, +, t, t, []() { \
    return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##ADD, nullptr)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, -, t, t, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##SUB, nullptr)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, *, t, t, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##MUL, nullptr)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, /, t, t, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##DIV, nullptr)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, <, t, bool, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##LT, nullptr), new ByteResolver(false)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, >, t, bool, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##GT, nullptr), new ByteResolver(false)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, <=, t, bool, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##LT, nullptr), new ByteResolver(true)}); \
    }) \
    BYTES_OPERATOR_DECLARATION(t, >=, t, bool, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_##prefix##GT, nullptr), new ByteResolver(true)}); \
    })

Compiler::Compiler(std::vector<Stmt *> stmts) : stmts(std::move(stmts)) {
    frame = new Frame();
    typesManager = new TypesManager();

    initChunk(&chunk);

    auto TYPE_ENTRY(int) = frame->types.add(new IdentifierTypeDescriptor("int", new ScalarTypeDefinition("int")));
    auto TYPE_ENTRY(double) = frame->types.add(
            new IdentifierTypeDescriptor("double", new ScalarTypeDefinition("double")));
    auto TYPE_ENTRY(bool) = frame->types.add(new IdentifierTypeDescriptor("bool", new ScalarTypeDefinition("bool")));
    auto TYPE_ENTRY(string) = frame->types.add(
            new IdentifierTypeDescriptor("string", new ScalarTypeDefinition("string")));

    BYTES_OPERATOR_MATH_DECLARATIONS(int, I)

    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(int, double, double)
    NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(int)

    BYTES_OPERATOR_DECLARATION(int, %, int, int, []() { \
        return std::vector<ByteResolver *>({new ByteResolver(OP_IMOD, nullptr)}); \
    })

    NATIVE_UNARY_OPERATOR_MATH_DECLARATIONS(int, int)

    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(double, double, double)
    NATIVE_BINARY_OPERATOR_MATH_DECLARATIONS(double, int, double)
    NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(double)

    NATIVE_UNARY_OPERATOR_MATH_DECLARATIONS(double, double)

    NATIVE_BINARY_OPERATOR_STRING_DECLARATIONS(bool)

    NATIVE_BINARY_OPERATOR_DECLARATION(string, +, string, string, add)

    auto FUNCTION_ENTRY_VAR(bool, unary_prefix_bool_invert) = ENTRY_DEF(TYPE_ENTRY(bool))->addPrefix(
            SELF_RECEIVER("right", bool),
            new NativeFunctionEntry(
                    "!",
                    new FunctionTypeDescriptor(
                            true, \
                            {},
                            {TYPE_DESC(bool)}
                    ),
                    unary_prefix_bool_invert
            )
    );
    REGISTER_FUNCTION(bool, unary_prefix_bool_invert);

    NATIVE_PRINT(int)
    NATIVE_PRINT(double)
    NATIVE_PRINT(bool)
    NATIVE_PRINT(string)

    frame->functions.add(
            new NativeFunctionEntry(
                    "vm_stats",
                    new FunctionTypeDescriptor(false, {}, {}),
                    vm_stats
            )
    );

    frame->functions.add(
            new NativeFunctionEntry(
                    "gc",
                    new FunctionTypeDescriptor(false, {}, {}),
                    run_gc
            )
    );
}

Chunk Compiler::compile() {
    for (auto stmt: stmts) {
        stmt->symbolize(this);
    }

    typesManager->link();

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

    typesManager->generateVEntries(this);

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
