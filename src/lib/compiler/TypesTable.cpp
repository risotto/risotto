//
// Created by rvigee on 10/4/19.
//

#include "TypesTable.h"
#include "TypeDefinition.h"
#include <utility>

TypeEntry *TypesTable::findNamed(const std::string &name) {
    for (auto entry : entries) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

TypeEntry *TypesTable::add(const std::string& name) {
    auto entry = new ConcreteTypeDefinition(name);

    return add(name, entry);
}

TypeDefinition *TypesTable::add(TypeDefinition *typeDefinition) {
    definitions.push_back(typeDefinition);

    return typeDefinition;
}

TypeEntry *TypesTable::add(const std::string& name, TypeDefinition *typeDefinition) {
    typeDefinition = add(typeDefinition);

    auto entry = new TypeEntry(name, typeDefinition);
    entries.push_back(entry);

    return entry;
}

TypeDefinition *TypesTable::addVirtual(const std::string &id, TypeDefinition *typeDefinition) {
    virtualEntries.insert(std::make_pair(id, typeDefinition));

    return typeDefinition;
}

TypeDefinition *TypesTable::findVirtual(const std::string &name) {
    auto it = virtualEntries.find(name);

    if (it != virtualEntries.end()) {
        return it->second;
    }

    return nullptr;
}

TypeEntry::TypeEntry(std::string name, TypeDefinition *definition) : name(std::move(name)), definition(definition) {}
