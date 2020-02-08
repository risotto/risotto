#include "stats.h"

void stats_init(Stats *stats, unsigned int maxopcode) {
    stats->opsc = 0;

    for (int m = 0; m <= maxopcode; ++m) {
        stats->timings[m] = 0;
        stats->timingsc[m] = 0;
    }
}

void stats_op(Stats *stats, OpCode op, unsigned long long time) {
    stats->timings[op] += time;
    stats->timingsc[op]++;
    stats->opsc++;
}
