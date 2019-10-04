//
// Created by rvigee on 10/3/19.
//

#include "Compiler.h"

#include <utility>

extern "C" {
#include <lib/vm/native_functions.h>
}

Compiler::Compiler(std::vector<Stmt *> stmts) : stmts(std::move(stmts)) {
    frame = new Frame();

    initChunk(&chunk);

    auto voidEntry = frame->types.add("void");
    auto intEntry = frame->types.add("int");

    intEntry->operators.add(
            new NativeFunctionEntry(
                    "+",
                    {FunctionEntryParameter("right", intEntry)},
                    intEntry,
                    binary_int_add_int
            )
    );

    intEntry->operators.add(
            new NativeFunctionEntry(
                    "-",
                    {FunctionEntryParameter("right", intEntry)},
                    intEntry,
                    binary_int_sub_int
            )
    );

    intEntry->operators.add(
            new NativeFunctionEntry(
                    "==",
                    {FunctionEntryParameter("right", intEntry)},
                    intEntry,
                    binary_int_eq_int
            )
    );

    frame->functions.add(
            new NativeFunctionEntry(
                    "println",
                    {FunctionEntryParameter("i", intEntry)},
                    voidEntry,
                    println_int
            )
    );
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
