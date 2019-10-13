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

    virtual ~FunctionEntry() = default;
};

class NativeFunctionEntry: public FunctionEntry {
public:
    NativeFunctionReturn (*fun)(Value[], int);

    NativeFunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, ReturnTypes returnTypes, NativeFunctionReturn (*fun)(Value[], int));
};

class FunctionsTable {
private:
    std::vector<FunctionEntry *> entries;
public:
    FunctionEntry *find(const std::string &name, const std::vector<TypeReference *>& argsTypes);
    std::vector<FunctionEntry *> findCandidates(const std::string &name);

    FunctionEntry * add(FunctionEntry *entry);
};


#endif //RISOTTOV2_FUNCTIONSTABLE_H
