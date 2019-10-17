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

class FunctionTypeReference;

class Frame;

class TypeReference {
public:
    virtual bool canReceiveType(TypeReference *other) = 0;

    virtual FunctionTypeReference *asFunction();

    virtual std::string toString() = 0;

    virtual TypeDefinition *toTypeDefinition(Frame *frame) = 0;
};

class ReceiverTypeReference {
public:
    virtual std::vector<FunctionEntry *> findFunctionsCandidates(Frame *frame, const std::string &name) = 0;

    virtual FunctionEntry *
    findOperator(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual FunctionEntry *
    findPrefix(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) = 0;
};

class ArrayTypeReference : public TypeReference, public ReceiverTypeReference {
public:
    TypeReference *element;

    explicit ArrayTypeReference(TypeReference *element);

    std::vector<FunctionEntry *> findFunctionsCandidates(Frame *frame, const std::string &name) override;

    FunctionEntry *
    findOperator(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    TypeDefinition *toTypeDefinition(Frame *frame) override;

    std::string toString() override;
};

class FunctionTypeReferenceParameter {
public:
    std::string name;
    TypeReference *type;
    bool asReference;

    FunctionTypeReferenceParameter(std::string name, TypeReference *type, bool asReference);
};

class FunctionTypeReference : public TypeReference {
public:
    std::vector<FunctionTypeReferenceParameter> params;
    std::vector<TypeReference *> returnTypes;

    FunctionTypeReference(std::vector<FunctionTypeReferenceParameter> params,
                          std::vector<TypeReference *> returnTypes);

    bool canReceiveType(TypeReference *other) override;

    FunctionTypeReference *asFunction() override;

    TypeDefinition *toTypeDefinition(Frame *frame) override;

    std::string toString() override;
};

class ConcreteTypeReference : public TypeReference, public ReceiverTypeReference {
public:
    TypeDefinition *definition;

    explicit ConcreteTypeReference(TypeDefinition *entry);

    std::vector<FunctionEntry *> findFunctionsCandidates(Frame *frame, const std::string &name) override;

    FunctionEntry *
    findOperator(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    std::string toString() override;

    TypeDefinition *toTypeDefinition(Frame *frame) override;
};

class NamedTypeReference : public ConcreteTypeReference {
public:
    std::string name;

    explicit NamedTypeReference(std::string name, TypeDefinition *entry);

    std::string toString() override;
};

class StructTypeReference : public TypeReference {
public:
    StructTypeDefinition *definition;

    explicit StructTypeReference(StructTypeDefinition *entry);

    bool canReceiveType(TypeReference *other) override;

    std::string toString() override;

    TypeDefinition *toTypeDefinition(Frame *frame) override;
};

#endif //RISOTTOV2_TYPEREFERENCE_H
