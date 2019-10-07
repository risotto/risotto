//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"
#include "TypesTable.h"

TypeReference::TypeReference(TypeEntry *type, bool isArray) : type(type), isArray(isArray) {}

bool TypesEntries::single() {
    return size() == 1;
}

TypesEntries::TypesEntries(TypeEntry *entry) : vector<TypeEntry *>({entry}) {
}

TypesEntries TypesEntries::onlyFunctions() {
    auto onlyFunctions = TypesEntries();

    for (auto entry : *this) {
        if (entry->isFunction()) {
            onlyFunctions.push_back(entry);
        }
    }

    return onlyFunctions;
}