//
// Created by raphael on 10/04/18.
//

#include "common.h"
#include "vm.h"
#include <stdio.h>
#include <assert.h>
#include "value.h"
#include "../lib/vec/src/vec.h"

#ifdef DEBUG_TRACE_EXECUTION
#include "debug.h"
#endif

#ifdef BENCHMARK_TIMINGS

#include <time.h>
#include "debug.h"

#endif

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.object.values[READ_BYTE()])
#define VM_GOTO(a) vm.ip = vm.chunk->code + (a)

VM vm;

static void resetStack() {
    vm.sp = vm.stack;
    vm.fp = vm.sp;
}

void initVM(unsigned int flags, int (*printf)(const char *, ...), ValueArray *args) {
    resetStack();
    vm.flags = flags;
    vm.numObjects = 0;
    vm.maxObjects = INITIAL_GC_THRESHOLD;
    vm.firstObject = NULL;
    vm.printf = printf;
    vm.args = args;
}

bool hasFlag(VMFlags flag) {
    return (vm.flags & flag) == flag;
}

VM *getVM() {
    return &vm;
}

void freeVM() {
    resetStack();
    gc();
    freeValueArray(vm.args);
}

void registerObject(Object *object) {
    if (vm.numObjects >= vm.maxObjects) gc();

    object->marked = 0;
    object->next = vm.firstObject;
    vm.firstObject = object;

    vm.numObjects++;
}

void markAll() {
    for (Value *v = vm.stack; v < vm.sp; v++) {
        markValue(v);
    }
}

void markValue(Value *v) {
    Value vl = *accessRefp(v);
    if (typecheck(vl, T_OBJECT) || typecheck(vl, T_ARRAY)) {
        Object *object = v2o(vl);

        mark(object);
    }
}

void mark(Object *object) {
    if (object->marked) return;

    object->marked = 1;

    for (int i = 0; i < object->size; ++i) {
        markValue(&object->values[i]);
    }
}

void sweep() {
    Object **object = &vm.firstObject;
    while (*object) {
        if (!(*object)->marked) {
            /* This object wasn't reached, so remove it from the list
               and free it. */
            Object *unreached = *object;

            *object = unreached->next;
            free(unreached);
            free(unreached->values);
            vm.numObjects--;
        } else {
            /* This object was reached, so unmark it (for the next GC)
               and move on to the next. */
            (*object)->marked = 0;
            object = &(*object)->next;
        }
    }
}

void gc() {
    markAll();
    sweep();

    vm.maxObjects = vm.numObjects < INITIAL_GC_THRESHOLD ? INITIAL_GC_THRESHOLD : vm.numObjects * 2;
}

#define VM_BINARY(code, f, rf, op) \
code##_label: { \
    OP_START_ROUTINE \
    Value l = pop(); \
    Value r = pop(); \
    push(rf##2v(v2##f(l) op v2##f(r))); \
} \
    OP_END_ROUTINE

#define VM_BINARY_EQ(code, f, op) \
code##_label: { \
    OP_START_ROUTINE \
    OP_T eq = READ_BYTE(); \
    Value l = pop(); \
    Value r = pop(); \
    if (eq) { \
        push(b2v(v2##f(l) op##= v2##f(r))); \
    } else { \
        push(b2v(v2##f(l) op v2##f(r))); \
    }\
}\
    OP_END_ROUTINE

#define VM_MATH_OPS(t, f) \
    VM_BINARY(OP_##t##ADD, f, f, +) \
    VM_BINARY(OP_##t##SUB, f, f, -) \
    VM_BINARY(OP_##t##MUL, f, f, *) \
    VM_BINARY(OP_##t##DIV, f, f, /) \
    VM_BINARY_EQ(OP_##t##LT, f, <) \
    VM_BINARY_EQ(OP_##t##GT, f, >)

#ifdef BENCHMARK_TIMINGS
#define BENCHMARK_TIMINGS_START_CODE \
            if (benchmarkExec) { \
                start = clock(); \
                opsc++; \
            }
#else
#define BENCHMARK_TIMINGS_START_CODE
#endif

#ifdef BENCHMARK_TIMINGS
#define BENCHMARK_TIMINGS_END_CODE \
        if (benchmarkExec) { \
            timings[*vm.ip] += clock() - start; \
            timingsc[*vm.ip]++; \
        }
#else
#define BENCHMARK_TIMINGS_END_CODE
#endif


#ifdef DEBUG_TRACE_EXECUTION
void trace_execution(bool traceExec) {
    static int i = 0;
    i++;
    if (traceExec && (i >12700000|| i < 10)) {
        printf("%i", i);
        printf("             ");
        for (Value *slot = vm.stack; slot < vm.sp; slot++) {
            if (vm.fp == slot) {
                printf("#");
            }

            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        long _ip = vm.ip - vm.chunk->code;
        long _sp = vm.sp - vm.stack;
        long _fp = vm.fp - vm.stack;

        printf(" IP: %lu SP: %lu FP: %lu", _ip, _sp, _fp);
        printf("\n");

        disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code) - 1);
    }
}
#define TRACE_EXECUTION_CODE trace_execution(traceExec);
#else
#define TRACE_EXECUTION_CODE
#endif

#define NEXT_OP goto *dispatch_table[READ_BYTE()]

#define OP_START_ROUTINE \
    TRACE_EXECUTION_CODE \
    BENCHMARK_TIMINGS_START_CODE

#define OP_END_ROUTINE \
    BENCHMARK_TIMINGS_END_CODE \
    NEXT_OP;

static InterpretResult run() {
#ifdef BENCHMARK_TIMINGS
    bool benchmarkExec = hasFlag(BenchmarkExecution);

    clock_t timings[Last + 1];
    unsigned long long timingsc[Last + 1];
    unsigned long long opsc = 0;

    for (int m = 0; m <= Last; ++m) {
        timings[m] = 0;
        timingsc[m] = 0;
    }

    clock_t start;
#endif

#ifdef DEBUG_TRACE_EXECUTION
    bool traceExec = hasFlag(TraceExecution);
#endif

    static const void *dispatch_table[] = {
            &&OP_CONST_label,
            &&OP_POP_label,
            &&OP_COPY_label,
            &&OP_NIL_label,
            &&OP_RETURN_label,
            &&OP_JUMP_label,
            &&OP_JUMPT_label,
            &&OP_JUMPF_label,
            &&OP_CALL_label,
            &&OP_END_label,
            &&OP_LOAD_label,
            &&OP_LOAD_LOCAL_label,
            &&OP_LOAD_STACK_label,
            &&OP_LOAD_INSTANCE_label,
            &&OP_NOOP_label,
            &&OP_NEW_label,
            &&OP_SET_label,
            &&OP_FRAME_label,
            &&OP_FRAME_END_label,
            &&OP_ARRAY_label,
            &&OP_TRUE_label,
            &&OP_FALSE_label,
            &&OP_EQ_label,
            &&OP_NEQ_label,
            &&OP_EQ_NIL_label,
            &&OP_NEQ_NIL_label,
            &&OP_RESOLVE_ADDR_label,
            &&OP_IADD_label,
            &&OP_ISUB_label,
            &&OP_IMUL_label,
            &&OP_IDIV_label,
            &&OP_ILT_label,
            &&OP_IGT_label,
            &&OP_IMOD_label,
            &&OP_DADD_label,
            &&OP_DSUB_label,
            &&OP_DMUL_label,
            &&OP_DDIV_label,
            &&OP_DLT_label,
            &&OP_DGT_label,
            &&OP_B_AND_label,
            &&OP_B_OR_label,
            &&OP_B_XOR_label,
            &&OP_B_SHIFTL_label,
            &&OP_B_SHIFTR_label,
            &&OP_B_NOT_label,
            &&OP_I2D_label,
            &&OP_D2I_label,
    };

    NEXT_OP;

    OP_NOOP_label:
    {
        OP_START_ROUTINE
    }
    OP_END_ROUTINE

    OP_CONST_label:
    {
        OP_START_ROUTINE
        Value constant = READ_CONSTANT();
        push(constant);
    }
    OP_END_ROUTINE
    OP_JUMP_label:
    {
        OP_START_ROUTINE
        OP_T addr = READ_BYTE();

        VM_GOTO(addr);

    }
    OP_END_ROUTINE
    OP_JUMPT_label:
    {
        OP_START_ROUTINE
        OP_T addr = READ_BYTE();
        bool b = v2b(pop());

        if (b == true) {
            VM_GOTO(addr);
        }
    }
    OP_END_ROUTINE
    OP_JUMPF_label:
    {
        OP_START_ROUTINE
        OP_T addr = READ_BYTE();
        int b = v2b(pop());

        if (b == false) {
            VM_GOTO(addr);
        }
    }
    OP_END_ROUTINE
    OP_RESOLVE_ADDR_label:
    {
        OP_START_ROUTINE
        OP_T vaddr = READ_BYTE();

        Value v = accessRef(pop());

#ifdef DEBUG_TRACE_EXECUTION
        if (traceExec) {
            printVtable(v);
       }
#endif
        if (v.vtable == NULL) {
            ERROR("vtable is null")
        }

        bool found = false;
        int i;
        vtable_entry *entry;
        vec_foreach_ptr(&v.vtable->addrs, entry, i) {
                if (entry->vaddr == vaddr) {
                    push(entry->addr);
                    found = true;
                    break;
                }
            }

        if (!found) {
            ERROR("Unable to find addr")
        }
    }
    OP_END_ROUTINE
    OP_CALL_label:
    {
        OP_START_ROUTINE
        int argc = READ_BYTE(); // args count
        int retc = READ_BYTE(); // return values count

        // we expect all args to be on the stack
        bool refs[argc];
        for (int i = 0; i < argc; ++i) {
            refs[i] = (bool) READ_BYTE();
        }

        Value f = accessRef(pop());

        switch (f.type) {
            case T_INT: { // Function
                int addr = v2i(f);

                push(i2v(argc));   // ... save num args ...
                push(p2v(vm.ip)); // ... save instruction pointer ...
                cframe();

                VM_GOTO(addr);

                for (int i = 0; i < argc; ++i) {
                    Value *a = vm.fp - 4 - i;

                    if (refs[i] == true) {
                        push(vp2v(a));
                    } else {
                        push(copy(*a));
                    }
                }

                break;
            }
            case T_P: { //  Native function
                Value args[argc];
                for (int i = 0; i < argc; ++i) {
                    args[i] = pop();
                }

                NativeFunction fun = v2p(f);

                Value returnValues[retc];
                fun(args, argc, returnValues);

                for (int i = 0; i < retc; ++i) {
                    push(returnValues[i]);
                }

                break;
            }
            default:
            ERROR("Unhandled function type")
        }

    }
    OP_END_ROUTINE
    OP_FRAME_label:
    {
        OP_START_ROUTINE
        cframe();
    }
    OP_END_ROUTINE
    OP_FRAME_END_label:
    {
        OP_START_ROUTINE
        dframe();
    }
    OP_END_ROUTINE
    OP_RETURN_label:
    {
        OP_START_ROUTINE
        OP_T d = READ_BYTE();
        OP_T rc = READ_BYTE();

        Value rvals[rc];
        for (int i = 0; i < rc; ++i) {
            rvals[i] = pop();     // pop return value from top of the stack
        }

        for (int j = 0; j < d; ++j) {
            dframe();
        }

        dframe();
        vm.ip = v2p(pop()); // restore ip
        int argc = v2i(pop());     // ... hom many args procedure has ...
        vm.sp -= argc;     // ... discard all of the args left ...

        // Reverse pop onto the stack to restablish order
        for (int i = rc - 1; i >= 0; --i) {
            push(copy(rvals[i])); // ... leave return value on top of the stack
        }

    }
    OP_END_ROUTINE
    OP_LOAD_label:
    {
        OP_START_ROUTINE
        OP_T addr = READ_BYTE();

        Value *vp = vm.fp + addr;

        push(vp2v(vp));
    }
    OP_END_ROUTINE
    OP_LOAD_STACK_label:
    {
        OP_START_ROUTINE
        OP_T addr = READ_BYTE();

        Value *v = vm.sp - 1 - addr;

        push(vp2v(v));
    }
    OP_END_ROUTINE
    OP_LOAD_LOCAL_label:
    {
        OP_START_ROUTINE
        OP_T dist = READ_BYTE();
        OP_T addr = READ_BYTE();

        Value *fp = vm.fp;
        for (int i = 0; i < dist; ++i) {
            Value *a = fp - 1;
            fp = v2p(*a);
        }

        Value *vp = fp + addr;

        assert(vp < vm.sp);
        assert(vp >= vm.stack);

        push(vp2v(vp));
    }
    OP_END_ROUTINE
    OP_LOAD_INSTANCE_label:
    {
        OP_START_ROUTINE
        OP_T addr = READ_BYTE();
        loadInstance(addr);
    }
    OP_END_ROUTINE
    OP_NEW_label:
    {
        OP_START_ROUTINE
        vtable *vtable = v2p(READ_CONSTANT());
        OP_T size = READ_BYTE();

        Object *instance = malloc(sizeof(*instance));
        instance->size = size;
        instance->values = malloc(size * sizeof(*instance->values));

        for (int i = 0; i < size; ++i) {
            instance->values[i] = n2v();
        }

        registerObject(instance);

        Value ov = o2v(instance);
        ov.vtable = vtable;

        push(ov);
    }
    OP_END_ROUTINE
    OP_SET_label:
    {
        OP_START_ROUTINE
        Value o = pop();
        Value *t = vm.sp - 1;

        set(o, t);

    }
    OP_END_ROUTINE
    OP_POP_label:
    {
        OP_START_ROUTINE
        int n = READ_BYTE();

        for (int i = 0; i < n; ++i) {
            popp();
        }

    }
    OP_END_ROUTINE
    OP_COPY_label:
    {
        OP_START_ROUTINE
        push(copy(pop()));
    }
    OP_END_ROUTINE
    OP_NIL_label:
    {
        OP_START_ROUTINE
        push(n2v());
    }
    OP_END_ROUTINE
    OP_ARRAY_label:
    {
        OP_START_ROUTINE
        int elemsc = READ_BYTE();

        ValueArray *array = malloc(sizeof(*array));
        initValueArray(array);
        registerObject((Object *) array);

        for (int i = 0; i < elemsc; ++i) {
            writeValueArray(array, pop());
        }

        push(a2v(array));
    }
    OP_END_ROUTINE
    OP_TRUE_label:
    {
        OP_START_ROUTINE
        push(b2v(true));
    }
    OP_END_ROUTINE
    OP_FALSE_label:
    {
        OP_START_ROUTINE
        push(b2v(false));
    }
    OP_END_ROUTINE
    OP_EQ_NIL_label:
    {
        OP_START_ROUTINE
        Value v = accessRef(pop());

        push(b2v(typecheck(v, T_NIL)));
    }
    OP_END_ROUTINE
    OP_NEQ_NIL_label:
    {
        OP_START_ROUTINE
        Value v = accessRef(pop());

        push(b2v(!typecheck(v, T_NIL)));
    }
    OP_END_ROUTINE
    OP_EQ_label:
    {
        OP_START_ROUTINE
        Value l = accessRef(pop());
        Value r = accessRef(pop());

        push(b2v(memcmp(&l.data, &r.data, sizeof(ValueData)) == 0));
    }
    OP_END_ROUTINE
    OP_NEQ_label:
    {
        OP_START_ROUTINE
        Value l = accessRef(pop());
        Value r = accessRef(pop());

        push(b2v(memcmp(&l.data, &r.data, sizeof(ValueData)) != 0));
    }
    OP_END_ROUTINE
VM_MATH_OPS(I, i)
VM_BINARY(OP_IMOD, i, i, %)
VM_MATH_OPS(D, d)
VM_BINARY(OP_B_AND, i, i, &)
VM_BINARY(OP_B_OR, i, i, |)
VM_BINARY(OP_B_XOR, i, i, ^)
VM_BINARY(OP_B_SHIFTL, i, i, <<)
VM_BINARY(OP_B_SHIFTR, i, i, >>)
    OP_B_NOT_label:
    {
        OP_START_ROUTINE
        Value l = pop();
        push(i2v(~v2i(l)));
    }
    OP_END_ROUTINE
    OP_I2D_label:
    {
        OP_START_ROUTINE
        Value l = pop();
        push(d2v(v2i(l)));
    }
    OP_END_ROUTINE
    OP_D2I_label:
    {
        OP_START_ROUTINE
        Value l = pop();
        push(i2v(v2d(l)));
    }
    OP_END_ROUTINE
    OP_END_label:
    {
#ifdef BENCHMARK_TIMINGS
        if (benchmarkExec) {
            printf("\n======================= TIMINGS =======================\n");
            clock_t tt = 0;
            for (int k = 0; k <= Last; ++k) {
                tt += timings[k];
            }

            for (int k = 0; k <= Last; ++k) {
                long double t = (long double) timings[k];
                unsigned long c = timingsc[k];

                if (c > 0) {
                    printf(
                            "%-3u - %-14s C: %-9lu T: %-10.0Lf AT: %-13.9Lf (%-5.2Lf%%) TT: %-13.9Lf \n",
                            k,
                            getName(k),
                            c,
                            t,
                            t / c / CLOCKS_PER_SEC,
                            (t / tt) * 100,
                            t / CLOCKS_PER_SEC
                    );
                }
            }
            printf("\n");
            printf("Total ops: %llu\n", opsc);
            printf("C: Count - T: CPU Ticks - AT: Average Time - TT: Total Time\n");
            printf("=======================================================\n");
        }
#endif
        return INTERPRET_OK;
    }
}

InterpretResult interpret(Chunk *chunk, long addr) {
    vm.chunk = chunk;
    vm.ip = vm.chunk->code + addr;

    InterpretResult result = run();
    return result;
}

void loadInstance(int index) {
    Object *array = v2o(pop());

    unsigned int offset;
    if (index < 0) {
        offset = index + array->size;
    } else {
        offset = index;
    }

    Value *p = array->values + offset;

    push(vp2v(p));
}

void set(Value origin, Value *target) {
    target = accessRefp(target);
    origin = accessRef(origin);

    target->data = origin.data;
    target->type = origin.type;
    target->vtable = origin.vtable;
}

void push(Value value) {
    if (vm.sp > vm.stack + STACK_MAX - 1) {
        ERROR("Stack overflow")
    }

    *vm.sp = value;
    vm.sp++;
}

void cframe() {
    push(p2v(vm.fp));
    vm.fp = vm.sp;
}

void dframe() {
    vm.sp = vm.fp;
    vm.fp = v2p(pop());
}

Value pop() {
    return *popp();
}

Value *popp() {
    if (vm.sp == vm.stack) {
        ERROR("Stack underflow")
    }

    return --vm.sp;
}
