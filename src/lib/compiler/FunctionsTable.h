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

class FunctionTypeDescriptor;

class FunctionEntry {
public:
    FunctionEntry(std::string name, FunctionTypeDescriptor *descriptor);

    std::string name;
    FunctionTypeDescriptor *descriptor;
    int addr = -1;

    ByteResolver *firstByte = nullptr;

    virtual ~FunctionEntry() = default;

    virtual bool isSame(FunctionEntry *other);
};

class DeclarationFunctionEntry : public FunctionEntry {
    using FunctionEntry::FunctionEntry;
};

class NativeFunctionEntry : public FunctionEntry {
public:
    NativeFunctionReturn (*fun)(Value[], int);

    NativeFunctionEntry(
            std::string name,
            FunctionTypeDescriptor *descriptor,
            NativeFunctionReturn (*fun)(Value[], int)
    );
};

class FunctionsTable {
public:
    std::vector<FunctionEntry *> entries;

    FunctionEntry *find(const std::string &name, const std::vector<TypeDescriptor *> &argsTypes);

    std::vector<FunctionEntry *> findCandidates(const std::string &name);

    FunctionEntry *add(FunctionEntry *entry);
};


#endif //RISOTTOV2_FUNCTIONSTABLE_H
