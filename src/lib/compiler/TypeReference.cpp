//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"

#include <utility>
#include "TypesTable.h"

TypeReferences::TypeReferences(TypeEntry *entry) : TypeReferences(new ConcreteTypeReference(entry)) {

}

TypeReferences::TypeReferences(TypeReference *ref) : TypeReferences({ref}) {
}

TypeReferences TypeReferences::onlyFunctions() {
    auto onlyFunctions = TypeReferences();

    for (auto ref : *this) {
        if (ref->isFunction()) {
            onlyFunctions.push_back(ref);
        }
    }

    return onlyFunctions;
}

ConcreteTypeReference::ConcreteTypeReference(TypeEntry *entry) : entry(entry) {}

bool ConcreteTypeReference::canReceiveType(TypeReference *other) {
    if (auto concrete = dynamic_cast<ConcreteTypeReference *>(other)) {
        return entry->canReceiveType(concrete->entry);
    }

    return false;
}

bool ConcreteTypeReference::isFunction() {
    return entry->isFunction();
}

FunctionEntry *ConcreteTypeReference::findOperator(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
    return entry->operators.find(name, types);
}

FunctionEntry *ConcreteTypeReference::findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
    return entry->prefixes.find(name, types);
}

std::string ConcreteTypeReference::toString() {
    return entry->name;
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

bool ArrayTypeReference::isFunction() {
    return false;
}

bool TypeReferences::single() {
    return size() == 1;
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

bool InterfaceTypeReference::isFunction() {
    return false;
}

std::string InterfaceTypeReference::toString() {
    return "func...";
}
