//
// Created by raphael on 10/04/18.
//

#ifndef RISOTTOVM_COMMON_H
#define RISOTTOVM_COMMON_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define OP_T uint16_t

#define ERROR(...) fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(EXIT_FAILURE);

#endif //RISOTTOVM_COMMON_H
