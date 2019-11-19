//
// Created by rvigee on 10/4/19.
//

#include "TypesTable.h"
#include "TypeDefinition.h"
#include <utility>
#include <cassert>

TypeEntry::TypeEntry(std::string name, TypeDefinition *definition) : name(std::move(name)), definition(definition) {}

TypeEntry::TypeEntry(TypeDefinition *definition): TypeEntry("", definition) {}

TypeEntry *TypesTable::findNamed(const std::string &name) {
    assert(!name.empty());

    for (auto entry : entries) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

TypeEntry *TypesTable::add(TypeEntry *typeEntry) {
    entries.push_back(typeEntry);

    return typeEntry;
}

TypeEntry *TypesTable::reg(const std::string &name) {
    return add(name, nullptr);
}

TypeEntry *TypesTable::add(const std::string &name, TypeDefinition *typeDefinition) {
    return add(new TypeEntry(name, typeDefinition));
}
