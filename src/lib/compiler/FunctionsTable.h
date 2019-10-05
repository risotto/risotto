//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_FUNCTIONSTABLE_H
#define RISOTTOV2_FUNCTIONSTABLE_H

#include <string>
#include <vector>
#include <lib/parser/nodes/ParameterDefinition.h>
#include <lib/vm/value.h>
#include "ByteResolver.h"

class TypeEntry;

class FunctionEntryParameter {
public:
    std::string name;
    TypeEntry *type;

    FunctionEntryParameter(const std::string &name, TypeEntry *type);
};

class FunctionEntry {
public:
    FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeEntry *returnType);

    std::string name;
    std::vector<FunctionEntryParameter> params;
    TypeEntry *returnType = nullptr;

    ByteResolver *firstByte = nullptr;

    virtual ~FunctionEntry() = default;
};

class NativeFunctionEntry: public FunctionEntry {
public:
    Value (*fun)(Value[], int);

    NativeFunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeEntry *returnType, Value (*fun)(Value[], int));
};

class FunctionsTable {
private:
    std::vector<FunctionEntry *> functions;
public:
    FunctionEntry *find(const std::string &name, std::vector<TypeEntry *> argsTypes);

    FunctionEntry * add(FunctionEntry *entry);
};


#endif //RISOTTOV2_FUNCTIONSTABLE_H