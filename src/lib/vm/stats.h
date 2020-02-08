//
// Created by rvigee on 2/8/20.
//

#ifndef RISOTTOV2_STATS_H
#define RISOTTOV2_STATS_H

#include "chunk.h"

typedef struct {
    unsigned long long opsc;
    unsigned long long timings[Last + 1];
    unsigned long long timingsc[Last + 1];
} Stats;

void stats_init(Stats *stats, unsigned int maxopcode);

void stats_op(Stats *stats, OpCode op, unsigned long long time);

#endif //RISOTTOV2_STATS_H
