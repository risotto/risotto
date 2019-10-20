//
// Created by rvigee on 10/11/19.
//

#ifndef RISOTTOV2_TypeDefinition_H
#define RISOTTOV2_TypeDefinition_H

extern "C" {
#include "lib/vm/value.h"
}

#include <string>
#include <vector>
#include "FunctionsTable.h"
#include "VariablesTable.h"

class TypeReference;

class TypeDefinition {
public:
    FunctionsTable functions = FunctionsTable();
    FunctionsTable operators = FunctionsTable();
    FunctionsTable prefixes = FunctionsTable();

    struct vtable *vtable;

    TypeDefinition();

    FunctionEntry *addFunction(const std::string &selfName, bool asReference, FunctionEntry *entry);

    FunctionEntry *addOperator(const std::string &selfName, bool asReference, FunctionEntry *entry);

    FunctionEntry *addPrefix(const std::string &selfName, bool asReference, FunctionEntry *entry);

    virtual TypeReference *getTypeReference() = 0;

    virtual ~TypeDefinition() = default;

protected:
    void addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry);
};

class ArrayTypeDefinition : public TypeDefinition {
public:
    TypeDefinition *element;

    explicit ArrayTypeDefinition(TypeDefinition *element);

    TypeReference *getTypeReference() override;
};

class ConcreteTypeDefinition : public TypeDefinition {
public:
    std::string name;

    explicit ConcreteTypeDefinition(std::string name);

    TypeReference *getTypeReference() override;
};

class FunctionTypeDefinition : public TypeDefinition {
public:
    FunctionEntry *entry;

    explicit FunctionTypeDefinition(FunctionEntry *function);

    TypeReference *getTypeReference() override;
};

class StructTypeDefinition : public TypeDefinition {
public:
    VariablesTable fields;
    std::vector<FunctionEntry *> constructors;

    explicit StructTypeDefinition(VariablesTable fields);

    FunctionEntry *addConstructor(const std::string &selfName, bool asReference, FunctionEntry *entry);

    int getFieldIndex(VariableEntry *entry);

    TypeReference *getTypeReference() override;
};

class InterfaceTypeDefinition : public TypeDefinition {
public:
    TypeReference *getTypeReference() override;
};


#endif //RISOTTOV2_TypeDefinition_H
