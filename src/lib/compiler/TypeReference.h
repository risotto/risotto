//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>
#include <string>

class Compiler;

class FunctionEntry;

class InterfaceFunctionEntry;

class TypeDefinition;

class StructTypeDefinition;

class FunctionTypeDefinition;

class TypeReference {
private:
    TypeDefinition *typeDefinition;

public:
    TypeReference();

    explicit TypeReference(TypeDefinition *typeDefinition);

    virtual bool canReceiveType(TypeReference *other) = 0;

    virtual std::string toString() = 0;

    virtual TypeDefinition *computeTypeDefinition(Compiler *compiler) = 0;

    TypeDefinition *getTypeDefinition();

    TypeDefinition *getTypeDefinition(Compiler *compiler);
};

class ArrayTypeReference : public TypeReference {
public:
    TypeReference *element;

    explicit ArrayTypeReference(TypeReference *element);

    std::string toString() override;

    TypeDefinition *computeTypeDefinition(Compiler *compiler) override;

    bool canReceiveType(TypeReference *other) override;
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

    FunctionTypeReference(
            std::vector<FunctionTypeReferenceParameter> params,
            std::vector<TypeReference *> returnTypes
    );

    explicit FunctionTypeReference(FunctionEntry *entry);

    std::string toString() override;

    TypeDefinition *computeTypeDefinition(Compiler *compiler) override;

    bool canReceiveType(TypeReference *other) override;
};

class NamedTypeReference : public TypeReference {
public:
    std::string name;

    explicit NamedTypeReference(std::string name, TypeDefinition *def);

    std::string toString() override;

    TypeDefinition *computeTypeDefinition(Compiler *compiler) override;

    bool canReceiveType(TypeReference *other) override;
};

class StructTypeReference : public TypeReference {
public:
    explicit StructTypeReference(StructTypeDefinition *def);

    std::string toString() override;

    TypeDefinition *computeTypeDefinition(Compiler *compiler) override;

    bool canReceiveType(TypeReference *other) override;
};

class InterfaceTypeReference : public TypeReference {
public:
    std::vector<std::pair<std::string, FunctionTypeReference *>> functions;

    explicit InterfaceTypeReference(std::vector<std::pair<std::string, FunctionTypeReference *>> functions);

    std::string toString() override;

    TypeDefinition *computeTypeDefinition(Compiler *compiler) override;

    bool canReceiveType(TypeReference *other) override;
};

#endif //RISOTTOV2_TYPEREFERENCE_H
