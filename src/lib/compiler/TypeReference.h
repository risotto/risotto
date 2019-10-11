//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>
#include <string>

class TypeEntry;
class FunctionTypeEntry;

class FunctionEntry;

class Compiler;

class TypeReference {
public:
    virtual FunctionEntry *
    findFunction(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) = 0;

    virtual bool canReceiveType(TypeReference *other) = 0;

    virtual bool isFunction();

    virtual std::string toString() = 0;
};

class ArrayTypeReference : public TypeReference {
public:
    TypeReference *element;

    explicit ArrayTypeReference(TypeReference *element);

    FunctionEntry *
    findFunction(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    std::string toString() override;
};

class FunctionTypeReference : public TypeReference {
public:
    FunctionTypeEntry *entry;

    explicit FunctionTypeReference(FunctionTypeEntry *entry);

    FunctionEntry *
    findFunction(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool canReceiveType(TypeReference *other) override;

    bool isFunction() override;

    std::string toString() override;
};

class InterfaceTypeReference : public TypeReference {
public:
    class Function {
        std::string name;
        std::vector<TypeReference *> arguments;
        TypeReference *returnType;

    public:
        Function(std::string name, std::vector<TypeReference *> arguments, TypeReference *returnType);
    };

    std::vector<Function *> functions;

    explicit InterfaceTypeReference(std::vector<Function *> functions);

    bool canReceiveType(TypeReference *other) override;

    FunctionEntry *
    findFunction(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    std::string toString() override;
};

class NamedTypeReference : public TypeReference {
public:
    TypeEntry *entry;

    explicit NamedTypeReference(TypeEntry *entry);

    bool canReceiveType(TypeReference *other) override;

    FunctionEntry *
    findFunction(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    FunctionEntry *
    findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) override;

    bool isFunction() override;

    std::string toString() override;
};

#endif //RISOTTOV2_TYPEREFERENCE_H
