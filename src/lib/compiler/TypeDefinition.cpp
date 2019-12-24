//
// Created by rvigee on 10/11/19.
//

#include "TypeDefinition.h"
#include "TypesTable.h"

#include <utility>
#include <lib/parser/nodes/TypeDescriptor.h>
#include <lib/compiler/utils/Utils.h>

bool TypeDefinition::canReceiveType(TypeDefinition *type) {
    return this == type;
}

void TypeDefinition::addSelf(ParameterDefinition *self, FunctionEntry *entry) {
    entry->descriptor->params.insert(entry->descriptor->params.begin(), {self});
}

FunctionEntry *TypeDefinition::addFunction(ParameterDefinition *self, FunctionEntry *entry) {
    addSelf(self, entry);

    return functions.add(entry);
}

FunctionEntry *TypeDefinition::addOperator(ParameterDefinition *self, FunctionEntry *entry) {
    addSelf(self, entry);

    return operators.add(entry);
}

FunctionEntry *TypeDefinition::addPrefix(ParameterDefinition *self, FunctionEntry *entry) {
    addSelf(self, entry);

    return prefixes.add(entry);
}

FunctionTypeDefinition::FunctionTypeDefinition(FunctionTypeDescriptor *descriptor) : descriptor(descriptor) {

}

bool FunctionTypeDefinition::canReceiveType(TypeDefinition *type) {
    if (TypeDefinition::canReceiveType(type)) {
        return true;
    }

    if (auto otherFunction = dynamic_cast<FunctionTypeDefinition *>(type)) {
        return Utils::typesMatch(this->descriptor->params, otherFunction->descriptor->params);
    }

    return false;
}

ArrayTypeDefinition::ArrayTypeDefinition(TypeDescriptor *element) : element(element) {

}

ScalarTypeDefinition::ScalarTypeDefinition(std::string name) : name(std::move(name)) {}

StructTypeDefinition::StructTypeDefinition(VariablesTable fields) : fields(std::move(fields)) {}

FunctionEntry *
StructTypeDefinition::addConstructor(ParameterDefinition *self, FunctionEntry *entry) {
    addSelf(self, entry);

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
