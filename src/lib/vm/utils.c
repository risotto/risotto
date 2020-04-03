//
// Created by rvigee on 4/3/20.
//

#include "utils.h"

uint64_t MAGIC() {
    static const int i = 1;
    // Takes of little / big endian
    bool bigEndian = ((*(char *) &i) == 0);
    return bigEndian ? 0x0102040810204080 : 0x8040201008040201;
}

uint8_t pack8b(bool *a) {
    uint64_t t = *((uint64_t *) a);
    return (MAGIC() * t >> 56) & 0xFF;
}

uint16_t pack16b(bool *a) {
    return ((uint16_t) pack8b(a + 0) << 8) | (pack8b(a + 8) << 0);
}

uint32_t pack32b(bool *a) {
    return ((uint32_t) pack16b(a + 0) << 16) | (pack16b(a + 16) << 0);
}

uint64_t pack64b(bool *a) {
    return ((uint64_t) pack32b(a + 0) << 32) | (pack32b(a + 32) << 0);
}

void unpack8b(bool *a, uint8_t v) {
    uint64_t mask = 0x8080808080808080ULL;
    *((uint64_t *) a) = ((MAGIC() * v) & mask) >> 7;
}

void unpack16b(bool *a, uint16_t v) {
    unpack8b(a + 0, v >> 8);
    unpack8b(a + 8, v >> 0);
}

void unpack32b(bool *a, uint32_t v) {
    unpack16b(a + 0, v >> 16);
    unpack16b(a + 16, v >> 0);
}

void unpack64b(bool *a, uint64_t v) {
    unpack32b(a + 0, v >> 32);
    unpack32b(a + 32, v >> 0);
}
