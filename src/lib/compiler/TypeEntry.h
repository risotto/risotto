//
// Created by rvigee on 10/11/19.
//

#ifndef RISOTTOV2_TYPEENTRY_H
#define RISOTTOV2_TYPEENTRY_H

#include <string>
#include <vector>
#include "FunctionsTable.h"

class TypeEntry {
public:
    TypeEntry(std::string name);

    std::string name;
    FunctionsTable functions;
    FunctionsTable operators;
    FunctionsTable prefixes;

    bool canReceiveType(TypeEntry *type);

    FunctionEntry *addFunction(const std::string &selfName, bool asReference, FunctionEntry *entry);

    FunctionEntry *addOperator(const std::string &selfName, bool asReference, FunctionEntry *entry);

    FunctionEntry *addPrefix(const std::string &selfName, bool asReference, FunctionEntry *entry);

    virtual ~TypeEntry() = default;

private:
    void addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry);
};

class ArrayTypeEntry : public TypeEntry {
public:
    TypeEntry *element;

    ArrayTypeEntry(std::string name, TypeEntry *element);
};

class InterfaceTypeEntry : public TypeEntry {
public:
    explicit InterfaceTypeEntry(std::string name);
};

class StructTypeEntry : public TypeEntry {
public:
    explicit StructTypeEntry(std::string name);
};

class PrimitiveTypeEntry : public TypeEntry {
public:
    explicit PrimitiveTypeEntry(std::string name);
};

class FunctionTypeEntry : public TypeEntry {
public:
    FunctionEntry *function;

    FunctionTypeEntry(std::string name, FunctionEntry *function);
};


#endif //RISOTTOV2_TYPEENTRY_H
