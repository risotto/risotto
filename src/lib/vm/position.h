//
// Created by rvigee on 1/12/20.
//

#ifndef RISOTTOV2_POSITION_H
#define RISOTTOV2_POSITION_H

#include <stdbool.h>

typedef struct {
    int line;
    int column;
} Position;

char *position_string(Position p);
bool position_equal(Position l, Position r);

#endif //RISOTTOV2_POSITION_H
