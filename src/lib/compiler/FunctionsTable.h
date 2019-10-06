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

class TypesEntries: public std::vector<TypeEntry *> {
public:
    TypesEntries(TypeEntry *entry);

    bool single();

    using std::vector<TypeEntry *>::vector;

    TypesEntries onlyFunctions();
};

class FunctionEntryParameter {
public:
    std::string name;
    TypeEntry *type;

    FunctionEntryParameter(std::string name, TypeEntry *type);
};

class FunctionTypeEntry;

class FunctionEntry {
public:
    FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypesEntries returnTypes);

    std::string name;
    std::vector<FunctionEntryParameter> params;
    TypesEntries returnTypes;
    FunctionTypeEntry *typeEntry = nullptr;

    ByteResolver *firstByte = nullptr;

    virtual ~FunctionEntry() = default;
};

class NativeFunctionEntry: public FunctionEntry {
public:
    NativeFunctionReturn (*fun)(Value[], int);

    NativeFunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypesEntries returnTypes, NativeFunctionReturn (*fun)(Value[], int));
};

class FunctionsTable {
private:
    std::vector<FunctionEntry *> functions;
public:
    FunctionEntry *find(const std::string &name, const std::vector<TypeEntry *>& argsTypes);
    std::vector<FunctionEntry *> findCandidates(const std::string &name);

    FunctionEntry * add(FunctionEntry *entry);
};


#endif //RISOTTOV2_FUNCTIONSTABLE_H
