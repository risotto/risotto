//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"
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

FunctionEntry *ConcreteTypeReference::findOperator(const std::string &name, const std::vector<TypeReference *> &types) {
    return entry->operators.find(name, types);
}

FunctionEntry *ConcreteTypeReference::findPrefix(const std::string &name, const std::vector<TypeReference *> &types) {
    return entry->prefixes.find(name, types);
}

std::string ConcreteTypeReference::toString() {
    return entry->name;
}

ArrayTypeReference::ArrayTypeReference(TypeReference *element): element(element) {

}

FunctionEntry *ArrayTypeReference::findOperator(const std::string &name, const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

FunctionEntry *ArrayTypeReference::findPrefix(const std::string &name, const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

bool ArrayTypeReference::canReceiveType(TypeReference *other) {
    return dynamic_cast<ArrayTypeReference *>(other) != nullptr;
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
