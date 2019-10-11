//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"

#include <utility>
#include "TypesTable.h"
#include "lib/compiler/ReturnTypes.h"

NamedTypeReference::NamedTypeReference(TypeEntry *entry) : entry(entry) {}

bool NamedTypeReference::canReceiveType(TypeReference *other) {
    if (auto concrete = dynamic_cast<NamedTypeReference *>(other)) {
        return entry->canReceiveType(concrete->entry);
    }

    return false;
}

FunctionEntry *NamedTypeReference::findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
    return entry->operators.find(name, types);
}

FunctionEntry *NamedTypeReference::findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
    return entry->prefixes.find(name, types);
}

std::string NamedTypeReference::toString() {
    return entry->name;
}

FunctionEntry *NamedTypeReference::findFunction(Compiler *compiler, const std::string &name,
                                                   const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

ArrayTypeReference::ArrayTypeReference(TypeReference *element): element(element) {

}

FunctionEntry *ArrayTypeReference::findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

FunctionEntry *ArrayTypeReference::findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

bool ArrayTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherArray = dynamic_cast<ArrayTypeReference *>(other)) {
        return element->canReceiveType(otherArray->element);
    }

    if (auto otherInterface = dynamic_cast<InterfaceTypeReference *>(other)) {
        return otherInterface->functions.empty();
    }

    return false;
}

std::string ArrayTypeReference::toString() {
    return "[]" + element->toString();
}

FunctionEntry *ArrayTypeReference::findFunction(Compiler *compiler, const std::string &name,
                                                const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

InterfaceTypeReference::Function::Function(std::string name, std::vector<TypeReference *> arguments,
                                           TypeReference *returnType) : name(std::move(name)), arguments(std::move(arguments)),
                                                                        returnType(returnType) {}

InterfaceTypeReference::InterfaceTypeReference(std::vector<Function *> functions): functions(std::move(functions)) {

}

bool InterfaceTypeReference::canReceiveType(TypeReference *other) {
    return functions.empty();
}

FunctionEntry *InterfaceTypeReference::findOperator(Compiler *compiler, const std::string &name,
                                                    const std::vector<TypeReference *> &types) {
    return nullptr;
}

FunctionEntry *InterfaceTypeReference::findPrefix(Compiler *compiler, const std::string &name,
                                                  const std::vector<TypeReference *> &types) {
    return nullptr;
}

std::string InterfaceTypeReference::toString() {
    return "func...";
}

FunctionEntry *InterfaceTypeReference::findFunction(Compiler *compiler, const std::string &name,
                                                    const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

bool TypeReference::isFunction() {
    return false;
}

bool NamedTypeReference::isFunction() {
    return TypeReference::isFunction();
}

FunctionTypeReference::FunctionTypeReference(FunctionTypeEntry *entry): entry(entry)  {

}

FunctionEntry *FunctionTypeReference::findFunction(Compiler *compiler, const std::string &name,
                                                   const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

FunctionEntry *FunctionTypeReference::findOperator(Compiler *compiler, const std::string &name,
                                                   const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

FunctionEntry *FunctionTypeReference::findPrefix(Compiler *compiler, const std::string &name,
                                                 const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

bool FunctionTypeReference::canReceiveType(TypeReference *other) {
    throw std::logic_error("Unimplemented");

}

bool FunctionTypeReference::isFunction() {
    return true;
}

std::string FunctionTypeReference::toString() {
    return "func () ... { ... }";
}
