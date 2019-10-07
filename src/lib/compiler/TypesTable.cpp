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

FunctionEntry *TypeEntry::addFunction(const std::string &selfName, bool isReference, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, this, isReference)});

    return functions.add(entry);
}

FunctionEntry *TypeEntry::addOperator(const std::string &selfName, bool isReference, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, this, isReference)});

    return operators.add(entry);
}

FunctionEntry *TypeEntry::addPrefix(const std::string &selfName, bool isReference, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, this, isReference)});

    return prefixes.add(entry);
}

FunctionEntry *TypeEntry::findFunction(const std::string &functionName, std::vector<TypeEntry *> argsTypes) {
    auto newArgsTypes = std::vector<TypeEntry *>(std::move(argsTypes));
    newArgsTypes.insert(newArgsTypes.begin(), this);

    return functions.find(functionName, newArgsTypes);
}

bool TypeEntry::isFunction() {
    return false;
}

FunctionTypeEntry *TypeEntry::asFunctionTypeEntry() {
    throw std::logic_error("TypeEntry isn't FunctionTypeEntry");
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

FunctionTypeEntry::FunctionTypeEntry(std::string name, FunctionEntry *function): TypeEntry(std::move(name)), function(function) {

}

bool FunctionTypeEntry::isFunction() {
    return true;
}

FunctionTypeEntry *FunctionTypeEntry::asFunctionTypeEntry() {
    return this;
}
