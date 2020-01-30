//
// Created by rvigee on 10/3/19.
//

#ifndef RISOTTOV2_FRAME_H
#define RISOTTOV2_FRAME_H


#include "VariablesTable.h"
#include "FunctionsTable.h"

class VariableFindResponse {
public:
    VariableEntry *variable;
    int distance;

    VariableFindResponse(VariableEntry *type, int distance);
};

enum FrameType {
    REGULAR,
    FUNCTION,
};

class Frame {
public:
    Frame *parent = nullptr;
    FrameType type = REGULAR;

    TypesTable types;
    VariablesTable variables;
    FunctionsTable functions;

    Frame() = default;

    explicit Frame(Frame *parent);

    explicit Frame(Frame *parent, FrameType type);

    TypeDescriptor *findNamedType(const std::string &name);

    TypeDescriptor *findType(TypeDescriptor *typeDesc);

    VariableFindResponse *findVariable(const std::string &name);

    std::vector<FunctionEntry *> findFunctionsCandidates(const std::string &name);

    FunctionEntry *findFunction(const std::string &name, std::vector<TypeDescriptor *> types);

    int findFrame(FrameType type);
};


#endif //RISOTTOV2_FRAME_H
