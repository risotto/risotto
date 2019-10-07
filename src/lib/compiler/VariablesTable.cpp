//
// Created by rvigee on 10/4/19.
//

#include "VariablesTable.h"

#include <utility>

VariableEntry::VariableEntry(std::string name, TypeEntry *typeEntry, int index) : name(std::move(name)),
                                                                                  type(typeEntry), index(index) {

}

VariableEntry *VariablesTable::find(const std::string &name) {
    for (auto entry : entries) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

VariableEntry *VariablesTable::add(std::string name, TypeEntry *typeEntry) {
    auto entry = new VariableEntry(std::move(name), typeEntry, entries.size());

    entries.push_back(entry);

    return entry;
}
