//
// Created by rvigee on 10/11/19.
//

#include "TypeDefinition.h"

#include <utility>

bool TypeDefinition::canReceiveType(TypeDefinition *type) {
    return this == type;
}

TypeReference *getTypeRef(TypeDefinition *typeDef) {
    if (auto arrayDef = dynamic_cast<ArrayTypeDefinition *>(typeDef)) {
        return new ArrayTypeReference(getTypeRef(arrayDef->element));
    }

    if (auto structDef = dynamic_cast<StructTypeDefinition *>(typeDef)) {
        return new StructTypeReference(structDef);
    }

    // Must be last
    if (dynamic_cast<ConcreteTypeDefinition *>(typeDef)) {
        return new ConcreteTypeReference(typeDef);
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

FunctionTypeDefinition::FunctionTypeDefinition(FunctionEntry *function) : function(function) {

}

ArrayTypeDefinition::ArrayTypeDefinition(TypeDefinition *element) : element(element) {

}

ConcreteTypeDefinition::ConcreteTypeDefinition(std::string name) : name(std::move(name)) {}

StructTypeDefinition::StructTypeDefinition(VariablesTable fields) : fields(std::move(fields)) {}

FunctionEntry *
StructTypeDefinition::addConstructor(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    addSelf(selfName, asReference, entry);

    constructors.push_back(entry);

    return entry;
}

int StructTypeDefinition::getFieldIndex(VariableEntry *entry) {
    auto i = 0;
    for (auto field : fields) {
        if (field == entry) {
            return i;
        }
        i++;
    }

    return -1;
}
