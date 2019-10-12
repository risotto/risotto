//
// Created by rvigee on 10/11/19.
//

#ifndef RISOTTOV2_TypeDefinition_H
#define RISOTTOV2_TypeDefinition_H

#include <string>
#include <vector>
#include "FunctionsTable.h"

class TypeDefinition {
public:
    FunctionsTable functions = FunctionsTable();
    FunctionsTable operators = FunctionsTable();
    FunctionsTable prefixes = FunctionsTable();

    bool canReceiveType(TypeDefinition *type);

    FunctionEntry *addFunction(const std::string &selfName, bool asReference, FunctionEntry *entry);

    FunctionEntry *addOperator(const std::string &selfName, bool asReference, FunctionEntry *entry);

    FunctionEntry *addPrefix(const std::string &selfName, bool asReference, FunctionEntry *entry);

    virtual ~TypeDefinition() = default;

private:
    void addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry);
};

class ArrayTypeDefinition : public TypeDefinition {
public:
    TypeDefinition *element;

    explicit ArrayTypeDefinition(TypeDefinition *element);

};

class ConcreteTypeDefinition : public TypeDefinition {
public:
    std::string name;

    explicit ConcreteTypeDefinition(std::string name);
};

class FunctionTypeDefinition : public TypeDefinition {
public:
    FunctionEntry *function;

    explicit FunctionTypeDefinition(FunctionEntry *function);
};


#endif //RISOTTOV2_TypeDefinition_H
