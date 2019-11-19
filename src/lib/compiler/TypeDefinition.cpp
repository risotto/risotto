//
// Created by rvigee on 10/11/19.
//

#include "TypeDefinition.h"
#include "TypesTable.h"

#include <utility>
#include <lib/parser/nodes/TypeDescriptor.h>

bool TypeDefinition::canReceiveType(TypeDefinition *type) {
    return this == type;
}

void TypeDefinition::addSelf(ParameterDefinition *self, FunctionEntry *entry) {
    entry->params.insert(entry->params.begin(), {self});
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

FunctionTypeDefinition::FunctionTypeDefinition(FunctionEntry *function) : entry(function) {

}

bool FunctionTypeDefinition::canReceiveType(TypeDefinition *type) {
    if (TypeDefinition::canReceiveType(type)) {
        return true;
    }

    if (auto otherFunction = dynamic_cast<FunctionTypeDefinition *>(type)) {
        if (this->entry->params.size() != otherFunction->entry->params.size()) {
            return false;
        }

        if (this->entry->returnTypes.size() != otherFunction->entry->returnTypes.size()) {
            return false;
        }

        for (int i = 0; i < this->entry->params.size(); ++i) {
            auto param = this->entry->params[i]->type;
            auto otherParam = otherFunction->entry->params[i]->type;

            if (!param->canReceiveType(otherParam)) {
                return false;
            }
        }

        for (int i = 0; i < this->entry->returnTypes.size(); ++i) {
            auto returnType = this->entry->returnTypes[i];
            auto otherReturnType = otherFunction->entry->returnTypes[i];

            if (!returnType->canReceiveType(otherReturnType)) {
                return false;
            }
        }

        return true;
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
