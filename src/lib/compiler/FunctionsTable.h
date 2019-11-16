//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_FUNCTIONSTABLE_H
#define RISOTTOV2_FUNCTIONSTABLE_H

extern "C" {
#include <lib/vm/value.h>
};

#include <string>
#include <vector>
#include <lib/parser/nodes/ParameterDefinition.h>
#include "ByteResolver.h"
#include "lib/compiler/ReturnTypes.h"

class TypeDefinition;
class TypeReference;

class FunctionEntryParameter {
public:
    std::string name;
    TypeReference * type;
    bool asReference;

    FunctionEntryParameter(std::string name, TypeReference * type);
    FunctionEntryParameter(std::string name, TypeReference * type, bool asReference);
};

class FunctionTypeDefinition;

class FunctionEntry {
public:
    FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, ReturnTypes returnTypes);

    std::string name;
    std::vector<FunctionEntryParameter> params;
    ReturnTypes returnTypes;
    // TODO: rename to definition
    FunctionTypeDefinition *typeDefinition = nullptr;

    ByteResolver *firstByte = nullptr;

    unsigned int addr = 0;

    virtual ~FunctionEntry() = default;
};

class NativeFunctionEntry: public FunctionEntry {
public:
    NativeFunctionReturn (*fun)(Value[], int);

    NativeFunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, ReturnTypes returnTypes, NativeFunctionReturn (*fun)(Value[], int));
};

class InterfaceFunctionEntry: public FunctionEntry {
public:
    using FunctionEntry::FunctionEntry;
};

class FunctionsTable {
private:
    std::vector<FunctionEntry *> entries;

    typedef std::vector<FunctionEntry *>::const_iterator iterator;

public:
    FunctionEntry *find(const std::string &name, const std::vector<TypeReference *>& argsTypes);
    std::vector<FunctionEntry *> findCandidates(const std::string &name);

    FunctionEntry * add(FunctionEntry *entry);

    [[nodiscard]] iterator begin() const { return entries.begin(); }

    [[nodiscard]] iterator end() const { return entries.end(); }

};


#endif //RISOTTOV2_FUNCTIONSTABLE_H
