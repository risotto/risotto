//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>
#include <string>

class TypeDefinition;

class FunctionTypeDefinition;

class StructTypeDefinition;

class FunctionEntry;

class Compiler;

class TypeReference {
public:
    virtual bool canReceiveType(TypeReference *other) = 0;

    virtual bool isFunction();

    virtual std::string toString() = 0;

    virtual TypeDefinition *toTypeDefinition(Compiler *compiler) = 0;
};

class ReceiverTypeReference {
public:
    virtual std::vector<FunctionEntry *> findFunctionsCandidates(Compiler *compiler, const std::string &name) = 0;

    virtual FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) = 0;
};

class ArrayTypeReference : public TypeReference, public ReceiverTypeReference {
public:
    TypeReference *element;

    explicit ArrayTypeReference(TypeReference *element);

    std::vector<FunctionEntry *> findFunctionsCandidates(Compiler *compiler, const std::string &name) override;

    FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    TypeDefinition *toTypeDefinition(Compiler *compiler) override;

    std::string toString() override;
};

class FunctionTypeReference : public TypeReference {
public:
    // TODO: rename to definition
    FunctionTypeDefinition *entry;

    explicit FunctionTypeReference(FunctionTypeDefinition *entry);

    bool canReceiveType(TypeReference *other) override;

    bool isFunction() override;

    TypeDefinition *toTypeDefinition(Compiler *compiler) override;

    std::string toString() override;
};

class ConcreteTypeReference : public TypeReference, public ReceiverTypeReference {
public:
    // TODO: Rename to definition
    TypeDefinition *entry;

    explicit ConcreteTypeReference(TypeDefinition *entry);

    std::vector<FunctionEntry *> findFunctionsCandidates(Compiler *compiler, const std::string &name) override;

    FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    std::string toString() override;

    TypeDefinition *toTypeDefinition(Compiler *compiler) override;
};

class NamedTypeReference : public ConcreteTypeReference {
public:
    std::string name;

    explicit NamedTypeReference(std::string name, TypeDefinition *entry);

    std::string toString() override;
};

class StructTypeReference : public TypeReference {
public:
    StructTypeDefinition *entry;

    explicit StructTypeReference(StructTypeDefinition *entry);

    bool canReceiveType(TypeReference *other) override;

    std::string toString() override;

    TypeDefinition *toTypeDefinition(Compiler *compiler) override;
};

#endif //RISOTTOV2_TYPEREFERENCE_H
