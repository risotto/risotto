//
// Created by rvigee on 10/11/19.
//

#include "TypeDefinition.h"

#include <utility>

bool TypeDefinition::canReceiveType(TypeDefinition *type) {
    return this == type;
}

TypeReference *getTypeRef(TypeDefinition *typeDef) {
    if (dynamic_cast<ConcreteTypeDefinition *>(typeDef)) {
        return new ConcreteTypeReference(typeDef);
    }

    if (auto array = dynamic_cast<ArrayTypeDefinition *>(typeDef)) {
        return new ArrayTypeReference(getTypeRef(array->element));
    }

    throw std::logic_error("Unhandled TypeDefinition");
}

void TypeDefinition::addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    auto typeRef = getTypeRef(this);
    entry->params.insert(entry->params.begin(), {FunctionEntryParameter(selfName, typeRef, asReference)});
}

FunctionEntry *TypeDefinition::addFunction(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    return functions.add(entry);
}

FunctionEntry *TypeDefinition::addOperator(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    return operators.add(entry);
}

FunctionEntry *TypeDefinition::addPrefix(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    return prefixes.add(entry);
}

FunctionTypeDefinition::FunctionTypeDefinition(FunctionEntry *function): function(function) {

}

ArrayTypeDefinition::ArrayTypeDefinition(TypeDefinition *element): element(element) {

}

ConcreteTypeDefinition::ConcreteTypeDefinition(std::string name) : name(std::move(name)) {}

