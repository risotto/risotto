//
// Created by rvigee on 10/8/19.
//

#ifndef RISOTTOV2_TYPEDESCRIPTOR_H
#define RISOTTOV2_TYPEDESCRIPTOR_H


#include <lib/tokenizer/Token.h>
#include <lib/compiler/TypeReference.h>

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

//class FunctionTypeDescriptor : public TypeDescriptor {
//public:
//    std::vector<TypeDescriptor *> parameters;
//    TypeDescriptor *returnType;
//
//    explicit FunctionTypeDescriptor(std::vector<TypeDescriptor *> parameters, TypeDescriptor *returnType);
//
//    TypeReference *toTypeReference(Compiler *compiler) override;
//
//    std::string toString() override;
//};

class InterfaceTypeDescriptor : public TypeDescriptor {
public:
    class Function {
    public:
        std::string name;
        std::vector<TypeDescriptor *> arguments;
        TypeDescriptor *returnType;

        Function(std::string name, std::vector<TypeDescriptor *> arguments, TypeDescriptor *returnType);
    };

    std::vector<Function *> functions;

    explicit InterfaceTypeDescriptor(std::vector<Function *> functions);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
};

class IdentifierTypeDescriptor : public TypeDescriptor {
public:
    Token *name;

    explicit IdentifierTypeDescriptor(Token *name);

    TypeReference *toTypeReference(Compiler *compiler) override;

    std::string toString() override;
};


#endif //RISOTTOV2_TYPEDESCRIPTOR_H
