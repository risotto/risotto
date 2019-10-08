//
// Created by rvigee on 10/4/19.
//

#include "VariablesTable.h"

#include <utility>

VariableEntry::VariableEntry(std::string name, TypeReference typeRef, int index) : name(std::move(name)),
                                                                                   typeRef(typeRef), index(index) {

}

VariableEntry *VariablesTable::find(const std::string &name) {
    for (auto entry : entries) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

VariableEntry *VariablesTable::add(std::string name, TypeReference typeRef) {
    auto entry = new VariableEntry(std::move(name), typeRef, entries.size());

    entries.push_back(entry);

    return entry;
}
