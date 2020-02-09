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
#include "stats.h"

#endif

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.object.values[READ_BYTE()])
#define VM_GOTO(a) vm.ip = vm.chunk->code + (a)

VM vm;

static void resetStack() {
    vm.sp = vm.stack;
    vm.fp = vm.sp;
}

void initVM(unsigned int flags, int (*printf)(const char *, ...), ValueArray *args, Stats *stats) {
    resetStack();
    vm.flags = flags;
    vm.numObjects = 0;
    vm.maxObjects = INITIAL_GC_THRESHOLD;
    vm.firstObject = NULL;
    vm.printf = printf;
    vm.args = args;
    vm.stats = stats;
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
    VM_BLOCK(code, { \
        Value l = pop(); \
        Value r = pop(); \
        push(rf##2v(v2##f(l) op v2##f(r))); \
    })

#define VM_BINARY_EQ(code, f, op) \
    VM_BLOCK(code, { \
        OP_T eq = READ_BYTE(); \
        Value l = pop(); \
        Value r = pop(); \
        if (eq) { \
            push(b2v(v2##f(l) op##= v2##f(r))); \
        } else { \
            push(b2v(v2##f(l) op v2##f(r))); \
        }\
    })

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
            }
#else
#define BENCHMARK_TIMINGS_START_CODE
#endif

#ifdef BENCHMARK_TIMINGS
#define BENCHMARK_TIMINGS_END_CODE \
        if (benchmarkExec) { \
            stats_op(vm.stats, *vm.ip, clock() - start); \
        }
#else
#define BENCHMARK_TIMINGS_END_CODE
#endif


#ifdef DEBUG_TRACE_EXECUTION

void trace_execution(bool traceExec) {
    static int i = 0;
    i++;
    if (traceExec && (i > 12700000 || i < 10)) {
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
    BENCHMARK_TIMINGS_END_CODE

#define VM_BLOCK(op, body) \
    INSTRUCT(op): \
        OP_START_ROUTINE \
        body \
        OP_END_ROUTINE \
        NEXT_OP;

#define INSTRUCT(op) lbl_##op

#define GOTO_LABEL(op) &&INSTRUCT(op),

InterpretResult run() {
#ifdef BENCHMARK_TIMINGS
    bool benchmarkExec = hasFlag(BenchmarkExecution);

    stats_init(vm.stats, Last);

    clock_t start;
#endif

#ifdef DEBUG_TRACE_EXECUTION
    bool traceExec = hasFlag(TraceExecution);
#endif

    static const void *dispatch_table[] = {
            OPCODES(GOTO_LABEL)
    };

    NEXT_OP; // Start execution

VM_BLOCK(OP_NOOP,)

VM_BLOCK(OP_CONST, {
    Value constant = READ_CONSTANT();
    push(constant);
})

VM_BLOCK(OP_JUMP, {
    OP_T addr = READ_BYTE();

    VM_GOTO(addr);
})

VM_BLOCK(OP_JUMPT, {
    OP_T addr = READ_BYTE();
    bool b = v2b(pop());

    if (b == true) {
        VM_GOTO(addr);
    }
})

VM_BLOCK(OP_JUMPF, {
    OP_T addr = READ_BYTE();
    bool b = v2b(pop());

    if (b == false) {
        VM_GOTO(addr);
    }
})

VM_BLOCK(OP_RESOLVE_ADDR, {
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
    vec_foreach_ptr(&v.vtable->addrs, entry, i)
    {
        if (entry->vaddr == vaddr) {
            push(entry->addr);
            found = true;
            break;
        }
    }

    if (!found) {
        ERROR("Unable to find addr")
    }
})

VM_BLOCK(OP_CALL, {
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
})

VM_BLOCK(OP_FRAME, {
    cframe();
})

VM_BLOCK(OP_FRAME_END, {
    dframe();
})

VM_BLOCK(OP_RETURN, {
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
})

VM_BLOCK(OP_LOAD, {
    OP_T addr = READ_BYTE();

    Value *vp = vm.fp + addr;

    push(vp2v(vp));
})

VM_BLOCK(OP_LOAD_STACK, {
    OP_T addr = READ_BYTE();

    Value *v = vm.sp - 1 - addr;

    push(vp2v(v));
})

VM_BLOCK(OP_LOAD_LOCAL, {
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
})

VM_BLOCK(OP_LOAD_INSTANCE, {
    OP_T addr = READ_BYTE();
    loadInstance(addr);
})

VM_BLOCK(OP_NEW, {
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
})

VM_BLOCK(OP_SET, {
    Value o = pop();
    Value *t = vm.sp - 1;

    set(o, t);
})

VM_BLOCK(OP_POP, {
    int n = READ_BYTE();

    for (int i = 0; i < n; ++i) {
        popp();
    }
})

VM_BLOCK(OP_COPY, {
    push(copy(pop()));
})

VM_BLOCK(OP_NIL, {
    push(n2v());
})

VM_BLOCK(OP_ARRAY, {
    int elemsc = READ_BYTE();

    ValueArray *array = malloc(sizeof(*array));
    initValueArray(array);
    registerObject((Object *) array);

    for (int i = 0; i < elemsc; ++i) {
        writeValueArray(array, pop());
    }

    push(a2v(array));
})

VM_BLOCK(OP_TRUE, {
    push(b2v(true));
})

VM_BLOCK(OP_FALSE, {
    push(b2v(false));
})

VM_BLOCK(OP_EQ_NIL, {
    Value v = accessRef(pop());

    push(b2v(typecheck(v, T_NIL)));
})

VM_BLOCK(OP_NEQ_NIL, {
    Value v = accessRef(pop());

    push(b2v(!typecheck(v, T_NIL)));
})

VM_BLOCK(OP_EQ, {
    Value l = accessRef(pop());
    Value r = accessRef(pop());

    push(b2v(memcmp(&l.data, &r.data, sizeof(ValueData)) == 0));
})

VM_BLOCK(OP_NEQ, {
    Value l = accessRef(pop());
    Value r = accessRef(pop());

    push(b2v(memcmp(&l.data, &r.data, sizeof(ValueData)) != 0));
})

VM_MATH_OPS(I, i)
VM_BINARY(OP_IMOD, i, i, %)
VM_BINARY(OP_B_AND, i, i, &)
VM_BINARY(OP_B_OR, i, i, |)
VM_BINARY(OP_B_XOR, i, i, ^)
VM_BINARY(OP_B_SHIFTL, i, i, <<)
VM_BINARY(OP_B_SHIFTR, i, i, >>)

VM_MATH_OPS(D, d)

VM_BLOCK(OP_B_NOT, {
    Value l = pop();
    push(i2v(~v2i(l)));
})

VM_BLOCK(OP_I2D, {
    Value l = pop();
    push(d2v(v2i(l)));
})

VM_BLOCK(OP_D2I, {
    Value l = pop();
    push(i2v(v2d(l)));
})

VM_BLOCK(OP_END, {
    return INTERPRET_OK;
})
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
