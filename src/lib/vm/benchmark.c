//
// Created by rvigee on 3/29/20.
//

#include "benchmark.h"
#include "vm.h"
#include "debug.h"

void benchmarkPrint(BenchmarkData *b) {
    bool benchmarkExec = hasFlag(BenchmarkExecution);

    if (!benchmarkExec) {
        return;
    }

    vm.printf("\n======================= TIMINGS =======================\n");
    unsigned long long tt = 0;
    for (int k = 0; k <= LastOpCode; ++k) {
        tt += b->timings[k];
    }

    for (int k = 0; k <= LastOpCode; ++k) {
        unsigned long long t = b->timings[k];
        unsigned long c = b->timingsc[k];

        if (c > 0) {
            vm.printf(
                    "%-3u - %-14s C: %-13lu AT: %-5llu (%-5.2Lf%%) TT: %-13llu \n",
                    k,
                    getName(k),
                    c,
                    t / c,
                    ((long double) t / tt) * 100,
                    t
            );
        }
    }
    vm.printf("\n");
    vm.printf("Total ops: %llu\n", b->opsc);
    vm.printf("C: Count - AT: Average Time - TT: Total Time\n");
    vm.printf("=======================================================\n");
}

unsigned long long nanosec(struct timespec t) {
    return (t.tv_sec * 1000000000) + t.tv_nsec;
}

void benchmarkRuntime(BenchmarkData *b) {
    bool benchmarkExec = hasFlag(BenchmarkExecution);

    if (!benchmarkExec) {
        return;
    }

    OP_T opcode = *(vm.ip);

    if (b->opsc > 0) {
        clock_gettime(CLOCK_MONOTONIC, &b->tend);

        b->timings[opcode] += nanosec(b->tend) - nanosec(b->tstart);
        b->timingsc[opcode]++;
    }

    b->opsc++;
    clock_gettime(CLOCK_MONOTONIC, &b->tstart);
}
