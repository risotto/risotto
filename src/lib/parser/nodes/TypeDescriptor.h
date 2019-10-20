//
// Created by rvigee on 10/8/19.
//

#ifndef RISOTTOV2_TYPEDESCRIPTOR_H
#define RISOTTOV2_TYPEDESCRIPTOR_H


#include <lib/tokenizer/Token.h>
#include <lib/compiler/TypeReference.h>
#include <lib/compiler/ReturnTypes.h>
#include "ParameterDefinition.h"

class Compiler;

class TypeDescriptor {
public:
    virtual TypeReference *toTypeReference(Compiler *compiler) = 0;

    virtual std::string toString() = 0;
};

class ArrayTypeDescriptor : public TypeDescriptor {
public:
    TypeDescriptor *element;

    explicit ArrayTypeDescriptor(TypeDescriptor *element);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
};

class StructTypeDescriptor : public TypeDescriptor {
public:
    class Field {
    public:
        Token * name;
        TypeDescriptor *type;

        Field(Token * name, TypeDescriptor *type);
    };

    std::vector<Field> fields;

    explicit StructTypeDescriptor(std::vector<Field> fields);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
private:
    StructTypeDefinition *typeDefinition = nullptr;
};

class IdentifierTypeDescriptor : public TypeDescriptor {
public:
    Token *name;

    explicit IdentifierTypeDescriptor(Token *name);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
};

class FunctionTypeDescriptor : public TypeDescriptor {
public:
    std::vector<ParameterDefinition> params;
    std::vector<TypeDescriptor *> returnTypes;

    FunctionTypeDescriptor(std::vector<ParameterDefinition> params, std::vector<TypeDescriptor *> returnTypes);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
};

class InterfaceTypeDescriptor : public TypeDescriptor {
public:
    std::vector<std::pair<Token *, FunctionTypeDescriptor *>> functions;

    explicit InterfaceTypeDescriptor(std::vector<std::pair<Token *, FunctionTypeDescriptor *>> functions);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
};

#endif //RISOTTOV2_TYPEDESCRIPTOR_H
