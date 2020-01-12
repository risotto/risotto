//
// Created by rvigee on 10/3/19.
//

#include "ByteResolver.h"

#include <utility>
#include <cstdint>

ByteResolver::ByteResolver(OP_T byte, Position *position) : final(true), byte(byte), position(position) {

}

ByteResolver::ByteResolver(OP_T byte): ByteResolver(byte, nullptr) {

}

ByteResolver::ByteResolver(std::function<OP_T(Compiler *c)> resolver, Position *position) : final(false), resolver(std::move(resolver)), position(position) {

}

void ByteResolver::finalize(Compiler *c) {
    if (final) {
        return;
    }

    byte = resolver(c);
    final = true;
}
