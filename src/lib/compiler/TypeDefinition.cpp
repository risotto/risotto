//
// Created by rvigee on 10/11/19.
//

#include "TypeDefinition.h"
#include <utility>

TypeDefinition::TypeDefinition() : vtable(new struct vtable) {
    vec_init(vtable);

    // TODO: unfake
    vtable_entry entry = {.vaddr  = 0, .addr= i2v(10)};
    vec_push(&vtable->addrs, entry);
}

void TypeDefinition::addSelf(const std::string &selfName, bool asReference, FunctionEntry *entry) {
    auto typeRef = getTypeReference();
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

FunctionTypeDefinition::FunctionTypeDefinition(FunctionEntry *function) : entry(function) {

}

TypeReference *FunctionTypeDefinition::getTypeReference() {
    return new FunctionTypeReference(entry);
}

ArrayTypeDefinition::ArrayTypeDefinition(TypeDefinition *element) : element(element) {

}

TypeReference *ArrayTypeDefinition::getTypeReference() {
    return new ArrayTypeReference(element->getTypeReference());
}

ConcreteTypeDefinition::ConcreteTypeDefinition(std::string name) : name(std::move(name)) {}

TypeReference *ConcreteTypeDefinition::getTypeReference() {
    return new NamedTypeReference(name, this);
}

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

TypeReference *StructTypeDefinition::getTypeReference() {
    return new StructTypeReference(this);
}

TypeReference *InterfaceTypeDefinition::getTypeReference() {
    auto functionRefs = std::vector<std::pair<std::string, FunctionTypeReference *>>();

    for (auto function : functions) {
        functionRefs.push_back(std::make_pair<std::string, FunctionTypeReference *>(
                static_cast<std::string>(function->name),
                new FunctionTypeReference(function)
        ));
    }

    return new InterfaceTypeReference(functionRefs);
}
