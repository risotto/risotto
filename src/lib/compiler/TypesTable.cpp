//
// Created by rvigee on 10/4/19.
//

#include "TypesTable.h"

#include <utility>

TypeEntry::TypeEntry(std::string name) : name(std::move(name)) {

}

TypeEntry *TypesTable::find(const std::string &name) {
    for (auto entry : types) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

TypeEntry *TypesTable::add(std::string name) {
    auto entry = new TypeEntry(std::move(name));

    types.push_back(entry);

    return entry;
}

