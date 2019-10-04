//
// Created by rvigee on 10/4/19.
//

#include "TypesTable.h"

#include <utility>

TypeEntry::TypeEntry(std::string name) : name(std::move(name)) {

}

bool TypeEntry::canReceiveType(TypeEntry *type) {
    return this == type;
}

FunctionEntry *TypeEntry::addFunction(const std::string &selfName, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, this)});

    return functions.add(entry);
}

FunctionEntry *TypeEntry::addOperator(const std::string &selfName, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, this)});

    return operators.add(entry);
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

