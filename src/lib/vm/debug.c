//
// Created by raphael on 10/04/18.
//

#include <stdio.h>

#include "types.h"
#include "debug.h"

void printValue(Value value) {
    switch (TGET(value)) {
        case T_P:
            printf("<P>");
            return;
        case T_NIL:
        case T_UINT:
        case T_INT:
        case T_DOUBLE:
        case T_ARRAY:
        case T_BOOL:
            printf("%s", v2s(value));
            return;
        case T_STR:
            printf("`%s`", v2s(value));
            return;
        case T_OBJECT: {
            Object *object = v2o(value);
            printf("O: %p {", object->values);
            for (int i = 0; i < object->size; ++i) {
                Value v = object->values[i];
                if (typecheck(v, T_OBJECT) && v2o(v) == object) {
                    printf("<self>");
                } else {
                    printValue(v);
                }

                if (i != object->size - 1) {
                    printf(", ");
                }
            }
            printf("}");
            return;
        }
        case T_VALUE_P: {
            printf("+");
            Value *vp = (Value *) DGET(value, p);
            printValue(*vp);
            return;
        }
    }

    printf("# Unknown type: %p #", &value);
}

void disassembleChunk(Chunk *chunk, const char *name) {
    printf("== %s ==\n", name);

    for (int i = 0; i < chunk->count;) {
        i = disassembleInstruction(chunk, i);
    }

    printf("== end %s ==\n", name);
}

static int simpleInstruction(const char *name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
    OP_T constant = chunk->code[offset + 1];
    printf("%-16s %4d '", name, constant);
    if (constant <= chunk->constants.object.size) {
        printValue(chunk->constants.object.values[constant]);
    } else {
        printf("# Constant '%d' not found #", constant);
    }
    printf("'\n");
    return offset + 2;
}

static int addrInstruction(const char *name, Chunk *chunk, int offset) {
    int addr = chunk->code[offset + 1];
    printf("%-16s => %4d\n", name, addr);
    return offset + 2;
}

static int biIntInstruction(const char *name, const char *l1, const char *l2, Chunk *chunk, int offset) {
    int dist = chunk->code[offset + 1];
    int addr = chunk->code[offset + 2];
    printf("%-11s %s:%-3d %s:%d\n", name, l1, dist, l2, addr);

    return offset + 3;
}

static int callInstruction(const char *name, Chunk *chunk, int offset) {
    OP_T argsc = chunk->code[offset + 1];
    OP_T retc = chunk->code[offset + 2];
//    OP_T refsn = chunk->code[offset + 3];

    printf("%-11s AC:%-3llu RC:%llu\n", name, argsc, retc);

    return offset + 4;
}

static int newInstruction(const char *name, Chunk *chunk, int offset) {
    int tcAddr = chunk->code[offset + 1];
    int c = chunk->code[offset + 2];
    printf("%-11s VTC: ", name);
    printValue(chunk->constants.object.values[tcAddr]);
    printf(" C:%-3d\n", c);

    return offset + 3 + c;
}

static int arrayInstruction(const char *name, Chunk *chunk, int offset) {
    int vtableAddr = chunk->code[offset + 1];
    int c = chunk->code[offset + 2];
    printf("%-11s C: %-3d VA: ", name, c);
    printValue(chunk->constants.object.values[vtableAddr]);
    printf("\n");

    return offset + 3 + c;
}

static int intInstruction(const char *name, Chunk *chunk, int offset) {
    int i = chunk->code[offset + 1];
    printf("%-16s %4d\n", name, i);

    return offset + 2;
}

#define OPCODE_NAME(op) case op: return #op;

char *getName(OP_T instruction) {
    switch (instruction) {
        OPCODES(OPCODE_NAME)
        default:
            return "Unknown opcode";
    }
}

int disassembleInstruction(Chunk *chunk, int offset) {
    printf("%04d ", offset);

    Position position = chunk->positions[offset];
    Position prevPosition = {};

    if (offset > 0) {
        prevPosition = chunk->positions[offset - 1];
    }

    if (position_equal(position, (Position) {}) || position_equal(position, prevPosition)) {
        printf("      | ");
    } else {
        printf("%7s ", position_string(position));
    }

    OP_T instruction = chunk->code[offset];
    switch (instruction) {
        case OP_IADD:
        case OP_ISUB:
        case OP_IMUL:
        case OP_IDIV:
        case OP_IMOD:
        case OP_ILT:
        case OP_ILTE:
        case OP_IGT:
        case OP_IGTE:
        case OP_DADD:
        case OP_DSUB:
        case OP_DMUL:
        case OP_DDIV:
        case OP_DLT:
        case OP_DLTE:
        case OP_DGT:
        case OP_DGTE:
        case OP_EQ:
        case OP_NEQ:
        case OP_SET:
        case OP_FRAME:
        case OP_FRAME_END:
            return simpleInstruction(getName(instruction), offset);
        case OP_CONST:
            return constantInstruction(getName(instruction), chunk, offset);
        case OP_RETURN:
            return simpleInstruction(getName(instruction), offset);
        case OP_JUMP:
            return addrInstruction(getName(instruction), chunk, offset);
        case OP_JUMPT:
            return addrInstruction(getName(instruction), chunk, offset);
        case OP_JUMPF:
            return addrInstruction(getName(instruction), chunk, offset);
        case OP_END:
            return simpleInstruction(getName(instruction), offset);
        case OP_LOAD:
            return intInstruction(getName(instruction), chunk, offset);
        case OP_RESOLVE_ADDR    :
            return intInstruction(getName(instruction), chunk, offset);
        case OP_LOAD_STACK:
            return intInstruction(getName(instruction), chunk, offset);
        case OP_LOAD_LOCAL:
            return biIntInstruction(getName(instruction), "D", "A", chunk, offset);
        case OP_LOAD_INSTANCE:
            return intInstruction(getName(instruction), chunk, offset);
        case OP_ARRAY:
            return arrayInstruction(getName(instruction), chunk, offset);
        case OP_CALL:
            return callInstruction(getName(instruction), chunk, offset);
        case OP_POP:
            return intInstruction(getName(instruction), chunk, offset);
        case OP_COPY:
            return simpleInstruction(getName(instruction), offset);
        case OP_NIL:
            return simpleInstruction(getName(instruction), offset);
        case OP_TRUE:
            return simpleInstruction(getName(instruction), offset);
        case OP_FALSE:
            return simpleInstruction(getName(instruction), offset);
        case OP_EQ_NIL:
            return simpleInstruction(getName(instruction), offset);
        case OP_NEQ_NIL:
            return simpleInstruction(getName(instruction), offset);
        case OP_NEW:
            return newInstruction(getName(instruction), chunk, offset);
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}

void printVtable(Value v) {
    if (VTGET(v) == NULL) {
        printf("<vtable null>\n");
        return;
    }

    printf("==== vtable %p ====\n", VTGET(v));
    printf("%-4s %-4s %s\n", "i", "va", "a");

    int i;
    vtable_entry *entry;
    vec_foreach_ptr(&VTGET(v)->addrs, entry, i) {
            printf("%-4d %-4d ", i, entry->vaddr);
            printValue(entry->addr);
            printf("\n");
        }

    printf("================\n");
}
