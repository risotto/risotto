//
// Created by rvigee on 4/3/20.
//

#ifndef RISOTTOV2_VM_UTILS_H
#define RISOTTOV2_VM_UTILS_H

#include "common.h"

uint8_t pack8b(bool *a);
uint16_t pack16b(bool *a);
uint32_t pack32b(bool *a);
uint64_t pack64b(bool *a);

void unpack8b(bool a[], uint8_t v);
void unpack16b(bool a[], uint16_t v);
void unpack32b(bool a[], uint32_t v);
void unpack64b(bool a[], uint64_t v);

#endif //RISOTTOV2_VM_UTILS_H
