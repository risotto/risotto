//
// Created by rvigee on 10/11/19.
//

#ifndef RISOTTOV2_TypeDefinition_H
#define RISOTTOV2_TypeDefinition_H

#include <string>
#include <vector>
#include "FunctionsTable.h"
#include "VariablesTable.h"

class TypeDefinition {
public:
    FunctionsTable functions = FunctionsTable();
    FunctionsTable operators = FunctionsTable();
    FunctionsTable prefixes = FunctionsTable();

    virtual bool canReceiveType(TypeDefinition *type);

    FunctionEntry *addFunction(ParameterDefinition *self, FunctionEntry *entry);

    FunctionEntry *addOperator(ParameterDefinition *self, FunctionEntry *entry);

    FunctionEntry *addPrefix(ParameterDefinition *self, FunctionEntry *entry);

    virtual ~TypeDefinition() = default;

protected:
    static void addSelf(ParameterDefinition *self, FunctionEntry *entry);
};

class ArrayTypeDefinition : public TypeDefinition {
public:
    TypeDescriptor *element;

    explicit ArrayTypeDefinition(TypeDescriptor *element);
};

class ScalarTypeDefinition : public TypeDefinition {
public:
    std::string name;

    explicit ScalarTypeDefinition(std::string name);
};

class FunctionTypeDefinition : public TypeDefinition {
public:
    FunctionTypeDescriptor *descriptor;

    explicit FunctionTypeDefinition(FunctionTypeDescriptor *descriptor);

    bool canReceiveType(TypeDefinition *type) override;
};

class StructTypeDefinition : public TypeDefinition {
public:
    VariablesTable fields;
    std::vector<FunctionEntry *> constructors;

    explicit StructTypeDefinition(VariablesTable fields);

    FunctionEntry *addConstructor(ParameterDefinition *self, FunctionEntry *entry);

    int getFieldIndex(VariableEntry *entry);
};


#endif //RISOTTOV2_TypeDefinition_H
