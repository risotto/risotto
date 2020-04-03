#include "trace.h"
#include "debug.h"
#include "vm.h"

void printTrace() {
    bool traceExec = hasFlag(TraceExecution);

    if (!traceExec) {
        return;
    }

    vm.printf("             ");
    for (Value *slot = vm.stack; slot < vm.sp; slot++) {
        if (vm.fp == slot) {
            vm.printf("#");
        }

        vm.printf("[ ");
        printValue(*slot);
        vm.printf(" ]");
    }
    long _ip = vm.ip - vm.chunk->code;
    long _sp = vm.sp - vm.stack;
    long _fp = vm.fp - vm.stack;

    vm.printf(" IP: %lu SP: %lu FP: %lu", _ip, _sp, _fp);
    vm.printf("\n");

    disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
}
