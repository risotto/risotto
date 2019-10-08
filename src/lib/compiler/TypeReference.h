//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>
#include <string>

class TypeEntry;

class FunctionEntry;

class TypeReference {
public:
    virtual FunctionEntry *findOperator(const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual FunctionEntry *findPrefix(const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual bool canReceiveType(TypeReference *other) = 0;

    virtual bool isFunction() = 0;

    virtual std::string toString() = 0;
};

class ArrayTypeReference : public TypeReference {
public:
    TypeReference *element;

    explicit ArrayTypeReference(TypeReference *element);

    FunctionEntry *findOperator(const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *findPrefix(const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    bool isFunction() override;

    std::string toString() override;
};

class ConcreteTypeReference : public TypeReference {
public:
    TypeEntry *entry;

    explicit ConcreteTypeReference(TypeEntry *entry);

    bool canReceiveType(TypeReference *other) override;

    FunctionEntry *findOperator(const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *findPrefix(const std::string &name, const std::vector<TypeReference *> &types) override;

    bool isFunction() override;

    std::string toString() override;
};


class TypeReferences : public std::vector<TypeReference *> {
public:
    using std::vector<TypeReference *>::vector;

    TypeReferences(TypeEntry *entry);

    TypeReferences(TypeReference *ref);

    bool single();

    TypeReferences onlyFunctions();
};

#endif //RISOTTOV2_TYPEREFERENCE_H
