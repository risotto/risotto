//
// Created by rvigee on 3/29/20.
//

#ifndef RISOTTOV2_BENCHMARK_H
#define RISOTTOV2_BENCHMARK_H

#include "chunk.h"
#include <time.h>

typedef struct {
    unsigned long long timings[LastOpCode + 1];
    unsigned long long timingsc[LastOpCode + 1];
    unsigned long long opsc;

    struct timespec tstart;
    struct timespec tend;
} BenchmarkData;

void benchmarkPrint(BenchmarkData *b);
void benchmarkRuntime(BenchmarkData *b);

#endif //RISOTTOV2_BENCHMARK_H
