//
// Created by rvigee on 10/11/19.
//

#include "TypeEntry.h"

#include <utility>

TypeEntry::TypeEntry(std::string name) : name(std::move(name)) {

}

bool TypeEntry::canReceiveType(TypeEntry *type) {
    return this == type;
}

void TypeEntry::addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, new NamedTypeReference(this), asReference)});
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

FunctionTypeEntry::FunctionTypeEntry(std::string name, FunctionEntry *function): TypeEntry(std::move(name)), function(function) {

}

ArrayTypeEntry::ArrayTypeEntry(std::string name, TypeEntry *element): TypeEntry(std::move(name)), element(element) {

}

InterfaceTypeEntry::InterfaceTypeEntry(std::string name) : TypeEntry(std::move(name)) {

}

StructTypeEntry::StructTypeEntry(std::string name) : TypeEntry(std::move(name)) {

}

PrimitiveTypeEntry::PrimitiveTypeEntry(std::string name) : TypeEntry(std::move(name)) {

}
