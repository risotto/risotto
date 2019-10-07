//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"
#include "TypesTable.h"

TypeReference::TypeReference(TypeEntry *type, bool isArray) : type(type), isArray(isArray) {}

bool TypeReferences::single() {
    return size() == 1;
}

TypeReferences::TypeReferences(TypeEntry *entry) : vector<TypeEntry *>({entry}) {
}

TypeReferences TypeReferences::onlyFunctions() {
    auto onlyFunctions = TypeReferences();

    for (auto entry : *this) {
        if (entry->isFunction()) {
            onlyFunctions.push_back(entry);
        }
    }

    return onlyFunctions;
}
