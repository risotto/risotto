//
// Created by rvigee on 10/4/19.
//

#include "TypesTable.h"

#include <utility>

TypeEntry *TypesTable::findNamed(const std::string &name) {
    for (auto entry : entries) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

TypeEntry *TypesTable::add(std::string name) {
    auto entry = new TypeEntry(std::move(name));

    entries.push_back(entry);

    return entry;
}
