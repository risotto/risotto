//
// Created by raphael on 10/04/18.
//

#include "common.h"
#include "vm.h"
#include <stdio.h>
#include <assert.h>
#include "value.h"
#include "debug.h"

#ifdef BENCHMARK_TIMINGS
#include <time.h>
#endif

#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.object.values[READ_BYTE()])
#define GOTO(a) vm.ip = vm.chunk->code + (a)

#define RED   "\x1B[31m"
#define RESET "\x1B[0m"

VM vm;

static void resetStack() {
    vm.sp = vm.stack;
    vm.fp = vm.sp;
}

void initVM() {
    resetStack();
    vm.numObjects = 0;
    vm.maxObjects = INITIAL_GC_THRESHOLD;
    vm.firstObject = NULL;
    vm.printf = &printf;
}

VM *getVM() {
    return &vm;
}

void freeVM() {
    resetStack();
    gc();
}

void registerObject(Object *object) {
    if (vm.numObjects >= vm.maxObjects) gc();

    object->next = vm.firstObject;
    vm.firstObject = object;

    vm.numObjects++;
}

void markAll() {
    for (Value *v = vm.stack; v < vm.sp; v++) {
        markValue(v);
    }
}

void markValue(Value *value) {
    Value v = followRefV(value);

    if (TYPECHECK(v, T_OBJECT) || TYPECHECK(v, T_ARRAY)) {
        Object *object = v2o(v);

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

static InterpretResult run() {
#ifdef BENCHMARK_TIMINGS
    clock_t timings[Last+1];
    unsigned long timingsc[Last+1];

    for (int m = 0; m <= Last; ++m) {
        timings[m] = 0;
        timingsc[m] = 0;
    }
#endif

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value *slot = vm.stack; slot < vm.sp; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        auto _ip = vm.ip - vm.chunk->code;
        auto _sp = vm.sp - vm.stack;
        auto _fp = vm.fp - vm.stack;

        printf(" IP: %lu SP: %lu FP: %lu", _ip, _sp, _fp);
        printf("\n");

        disassembleInstruction(vm.chunk, (int) (vm.ip - vm.chunk->code));
#endif

        OP_T instruction = READ_BYTE();

#ifdef BENCHMARK_TIMINGS
        clock_t start = clock();
#endif

        switch (instruction) {
            case OP_NOOP: {
                break;
            }
            case OP_CONST: {
                Value constant = READ_CONSTANT();
                push(copy(constant));
                break;
            }
            case OP_JUMP: {
                OP_T addr = READ_BYTE();

                GOTO(addr);

                break;
            }
            case OP_JUMPT: {
                OP_T addr = READ_BYTE();
                bool b = v2b(pop());

                if (b == true) {
                    GOTO(addr);
                }
                break;
            }
            case OP_JUMPF: {
                OP_T addr = READ_BYTE();
                int b = v2b(pop());

                if (b == false) {
                    GOTO(addr);
                }
                break;
            }
            case OP_CALL: {
                // we expect all args to be on the stack
                int addr = v2i(pop()); // get next instruction as an address of procedure jump ...
                int argc = READ_BYTE(); // ... and next one as number of arguments to load ...

                push(i2v(argc));   // ... save num args ...
                push(p2v(vm.ip)); // ... save instruction pointer ...
                cframe();
                GOTO(addr);

                for (int i = 0; i < argc; ++i) {
                    Value *a = vm.fp - 4 - i;
                    push(copy(*a));
                }

                break;
            }
            case OP_NATIVE_CALL: {
                NativeFunction fun = v2p(pop());
                int argc = READ_BYTE(); // ... and next one as number of arguments to load ...

                Value args[argc];
                for (int i = 0; i < argc; ++i) {
                    args[i] = pop();
                }

                Value returnValue = fun(args, argc);

                push(returnValue);

                break;
            }
            case OP_FRAME: {
                cframe();
                break;
            }
            case OP_FRAME_END: {
                dframe();
                break;
            }
            case OP_RETURN: {
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

                for (int i = 0; i < rc; ++i) {
                    push(copy(rvals[i])); // ... leave return value on top of the stack
                }

                break;
            }
            case OP_LOAD: {
                OP_T addr = READ_BYTE();

                Value *vp = vm.fp + addr;

                push(vp2v(vp));
                break;
            }
            case OP_LOAD_STACK: {
                OP_T addr = READ_BYTE();

                Value *v = vm.sp - 1 - addr;

                push(vp2v(v));
                break;
            }
            case OP_LOAD_LOCAL: {
                OP_T dist = READ_BYTE();
                OP_T addr = READ_BYTE();

                Value *fp = vm.fp;
                for (int i = 0; i < dist; ++i) {
                    fp = v2p(*(fp - 1));
                }

                Value *vp = fp + addr;

                assert(vp < vm.sp);
                assert(vp >= vm.stack);

                push(vp2v(vp));
                break;
            }
            case OP_LOAD_INSTANCE: {
                OP_T addr = READ_BYTE();
                loadInstance(addr);
                break;
            }
            case OP_DYNAMIC_LOAD_INSTANCE: {
                int index = v2i(pop());
                loadInstance(index);
                break;
            }
            case OP_NEW: {
                OP_T size = READ_BYTE();

                Object *instance = malloc(sizeof(*instance));
                instance->size = size;
                instance->values = malloc(size * sizeof(*instance->values));
                instance->marked = 0;

                for (int i = 0; i < size; ++i) {
                    instance->values[i] = n2v();
                }

                registerObject(instance);

                push(o2v(instance));
                break;
            }
            case OP_SET: {
                Value o = pop();
                Value *t = popp();

                set(o, t);
                push(vp2v(t));

                break;
            }
            case OP_POP: {
                popp();
                break;
            }
            case OP_COPY: {
                push(copy(pop()));
                break;
            }
            case OP_NIL: {
                push(n2v());
                break;
            }
            case OP_ARRAY: {
                int elemsc = READ_BYTE();

                ValueArray *array = malloc(sizeof(*array));
                initValueArray(array);
                registerObject((Object *) array);

                for (int i = 0; i < elemsc; ++i) {
                    writeValueArray(array, pop());
                }

                push(a2v(array));
                break;
            }
            case OP_TRUE: {
                push(b2v(true));
                break;
            }
            case OP_FALSE: {
                push(b2v(false));
                break;
            }
            case OP_EQ_NIL: {
                Value v = pop();

                push(b2v(typecheck(v, T_NIL)));
                break;
            }
            case OP_NEQ_NIL: {
                Value v = pop();

                push(b2v(!typecheck(v, T_NIL)));
                break;
            }
            case OP_PRINT: {
#ifdef PRINT_CARET
                printf(RED "> " RESET);
#endif
                vm.printf("%s", v2s(pop()));
                vm.printf("\n");
                break;
            }
            case OP_END: {
#ifdef BENCHMARK_TIMINGS
                printf("\n======================= TIMINGS =======================\n");
                for (int k = 0; k <= Last; ++k) {
                    clock_t t = timings[k];
                    unsigned long c = timingsc[k];

                    if(c > 0) {
                        printf("%-3u - %-14s C: %-6lu T: %-6lu AT: %-8Lf TT: %Lf \n", k, getName(k), c, t, ((long double)t)/c/CLOCKS_PER_SEC, ((long double)t)/CLOCKS_PER_SEC);
                    }
                }
                printf("C: Count - T: Ticks - AT: Average Time - TT: Total Time\n");
                printf("=======================================================\n");
#endif
                return INTERPRET_OK;
            }
            default:
                printf("Unknown op code: %d \n", instruction);
                return INTERPRET_RUNTIME_ERROR;
        }

#ifdef BENCHMARK_TIMINGS
        clock_t end = clock();
        timings[instruction] += end - start;
        timingsc[instruction]++;
#endif

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

    int offset;

    if (index < 0) {
        offset = index + array->size;
    } else {
        offset = index;
    }

    Value *p = array->values + offset;

    push(vp2v(p));
}

void set(Value origin, Value *target) {
    target = followRef(target);

    Value *rorigin = followRef(&origin);

    target->data = rorigin->data;
    target->type = rorigin->type;
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
