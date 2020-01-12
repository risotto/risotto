//
// Created by rvigee on 1/12/20.
//

#include <stdio.h>
#include <stdlib.h>
#include "position.h"

char *position_string(Position p) {
    char *s = malloc(10 * sizeof(char));
    sprintf(s, "%i:%i", p.line, p.column);

    return s;
}

bool position_equal(Position l, Position r) {
    return l.line == r.line && l.column == r.column;
}
