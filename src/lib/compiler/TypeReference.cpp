//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"
#include "TypesTable.h"

TypeReference::TypeReference(TypeEntry *entry, bool isArray) : entry(entry), isArray(isArray) {}


FunctionEntry *TypeReference::findOperator(const std::string& name, const std::vector<TypeReference>& types) {
    if (isArray) {
        throw std::logic_error("array not handled");
    }

    return entry->operators.find(name, types);
}

FunctionEntry *TypeReference::findPrefix(const std::string& name, const std::vector<TypeReference>& types) {
    if (isArray) {
        throw std::logic_error("array not handled");
    }

    return entry->prefixes.find(name, types);
}

bool TypeReference::canReceiveType(TypeReference other) {
    return entry->canReceiveType(other.entry) && isArray == other.isArray;
}

bool TypeReference::isFunction() {
    return !isArray && entry->isFunction();
}

bool TypeReferences::single() {
    return size() == 1;
}

TypeReferences::TypeReferences(TypeEntry *entry): TypeReferences(TypeReference(entry, false)) {

}

TypeReferences::TypeReferences(TypeReference ref) : vector<TypeReference>({ref}) {
}

TypeReferences TypeReferences::onlyFunctions() {
    auto onlyFunctions = TypeReferences();

    for (auto ref : *this) {
        if (ref.entry->isFunction()) {
            onlyFunctions.push_back(ref);
        }
    }

    return onlyFunctions;
}
