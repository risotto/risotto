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

void TypeEntry::addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, TypeReference(this, false), asReference)});
}

FunctionEntry *TypeEntry::addFunction(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    return functions.add(entry);
}

FunctionEntry *TypeEntry::addOperator(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    return operators.add(entry);
}

FunctionEntry *TypeEntry::addPrefix(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    return prefixes.add(entry);
}

bool TypeEntry::isFunction() {
    return false;
}

FunctionTypeEntry *TypeEntry::asFunctionTypeEntry() {
    throw std::logic_error("TypeEntry isn't FunctionTypeEntry");
}

TypeEntry *TypesTable::find(const std::string &name) {
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

FunctionTypeEntry::FunctionTypeEntry(std::string name, FunctionEntry *function): TypeEntry(std::move(name)), function(function) {

}

bool FunctionTypeEntry::isFunction() {
    return true;
}

FunctionTypeEntry *FunctionTypeEntry::asFunctionTypeEntry() {
    return this;
}
