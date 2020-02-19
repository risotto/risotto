//
// Created by raphael on 10/04/18.
//

#include <stdio.h>

#include "debug.h"

void printValue(Value value) {
    switch (TGET(value)) {
        case T_NIL:
        case T_P:
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
    printf("%-16s => %4d \n", name, addr);
    return offset + 2;
}

static int biIntInstruction(const char *name, const char *l1, const char *l2, Chunk *chunk, int offset) {
    int dist = chunk->code[offset + 1];
    int addr = chunk->code[offset + 2];
    printf("%-11s %s:%-3d %s:%-3d \n", name, l1, dist, l2, addr);

    return offset + 3;
}

static int callInstruction(const char *name, Chunk *chunk, int offset) {
    int argsc = chunk->code[offset + 1];
    int retc = chunk->code[offset + 2];
    printf("%-11s AC:%-3d RC:%-3d\n", name, argsc, retc);

    return offset + 3 + argsc;
}

static int newInstruction(const char *name, Chunk *chunk, int offset) {
    int vtableAddr = chunk->code[offset + 1];
    int c = chunk->code[offset + 2];
    printf("%-11s VA: ", name);
    printValue(chunk->constants.object.values[vtableAddr]);
    printf(" C:%-3d\n", c);

    return offset + 3 + c;
}

static int intInstruction(const char *name, Chunk *chunk, int offset) {
    int i = chunk->code[offset + 1];
    printf("%-16s %4d \n", name, i);

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
            return simpleInstruction(getName(instruction), offset);
        case OP_ILT:
        case OP_IGT:
            return intInstruction(getName(instruction), chunk, offset);
        case OP_DADD:
        case OP_DSUB:
        case OP_DMUL:
        case OP_DDIV:
            return simpleInstruction(getName(instruction), offset);
        case OP_DLT:
        case OP_DGT:
            return intInstruction(getName(instruction), chunk, offset);
        case OP_EQ:
            return simpleInstruction(getName(instruction), offset);
        case OP_NEQ:
            return simpleInstruction(getName(instruction), offset);
        case OP_SET:
            return simpleInstruction(getName(instruction), offset);
        case OP_FRAME:
            return simpleInstruction(getName(instruction), offset);
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
            return intInstruction(getName(instruction), chunk, offset);
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
    if (v.vtable == NULL) {
        printf("<null>\n");
        return;
    }

    printf("==== vtable %p ====\n", v.vtable);
    printf("%-4s %-4s %s\n", "i", "va", "a");

    int i;
    vtable_entry *entry;
    vec_foreach_ptr(&v.vtable->addrs, entry, i) {
            printf("%-4d %-4d ", i, entry->vaddr);
            printValue(entry->addr);
            printf("\n");
        }

    printf("================\n");
}
