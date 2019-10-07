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
#include "lib/compiler/TypeReference.h"

class TypeEntry;

class FunctionEntryParameter {
public:
    std::string name;
    TypeEntry *type;
    bool isReference;

    FunctionEntryParameter(std::string name, TypeEntry *type);
    FunctionEntryParameter(std::string name, TypeEntry *type, bool isReference);
};

class FunctionTypeEntry;

class FunctionEntry {
public:
    FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeReferences returnTypes);

    std::string name;
    std::vector<FunctionEntryParameter> params;
    TypeReferences returnTypes;
    FunctionTypeEntry *typeEntry = nullptr;

    ByteResolver *firstByte = nullptr;

    virtual ~FunctionEntry() = default;
};

class NativeFunctionEntry: public FunctionEntry {
public:
    NativeFunctionReturn (*fun)(Value[], int);

    NativeFunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeReferences returnTypes, NativeFunctionReturn (*fun)(Value[], int));
};

class FunctionsTable {
private:
    std::vector<FunctionEntry *> entries;
public:
    FunctionEntry *find(const std::string &name, const std::vector<TypeEntry *>& argsTypes);
    std::vector<FunctionEntry *> findCandidates(const std::string &name);

    FunctionEntry * add(FunctionEntry *entry);
};


#endif //RISOTTOV2_FUNCTIONSTABLE_H
