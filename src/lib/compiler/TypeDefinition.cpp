//
// Created by rvigee on 10/11/19.
//

#include "TypeDefinition.h"
#include "TypesTable.h"

#include <utility>
#include <lib/parser/nodes/TypeDescriptor.h>
#include <lib/compiler/utils/Utils.h>
#include <cassert>

bool TypeDefinition::canReceiveType(TypeDefinition *type) {
    return isSame(type) || isSame(&NilTypeDefinition::Def);
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

TypeDefinition::TypeDefinition() {}

TypeDefinition::TypeDefinition(const ValueTypeContainer *_vtc) : TypeDefinition() {
    assert(_vtc != nullptr);

    vtc = _vtc;
}

bool TypeDefinition::isSame(TypeDefinition *other) {
    return this == other;
}

const ValueTypeContainer *TypeDefinition::getVTC() {
    if (vtc == nullptr) {
        vtc = resolveVTC();
    }

    assert(vtc != nullptr);

    return vtc;
}

FunctionTypeDefinition::FunctionTypeDefinition(FunctionTypeDescriptor *descriptor)
        : TypeDefinition(), descriptor(descriptor) {

}

bool FunctionTypeDefinition::canReceiveType(TypeDefinition *type) {
    if (TypeDefinition::canReceiveType(type)) {
        return true;
    }

    if (auto otherFunction = dynamic_cast<FunctionTypeDefinition *>(type)) {
        return Utils::typesMatch(this->descriptor->params, otherFunction->descriptor->params, Utils::TypesCompatible);
    }

    return false;
}

bool FunctionTypeDefinition::isSame(TypeDefinition *other) {
    if (TypeDefinition::isSame(other)) {
        return true;
    }

    if (auto otherFunction = dynamic_cast<FunctionTypeDefinition *>(other)) {
        return descriptor->isSame(otherFunction->descriptor);
    }

    return false;
}

const ValueTypeContainer *FunctionTypeDefinition::resolveVTC() {
    auto tc = new ValueTypeContainer{.type = T_P, .vtable = new vtable};

    vec_init(tc->vtable);

    return tc;
}

ArrayTypeDefinition::ArrayTypeDefinition(TypeDescriptor *element)
        : TypeDefinition(), element(element) {}

bool ArrayTypeDefinition::isSame(TypeDefinition *other) {
    if (TypeDefinition::isSame(other)) {
        return true;
    }

    if (auto otherArray = dynamic_cast<ArrayTypeDefinition *>(other)) {
        return element->getTypeDefinition()->isSame(otherArray->element->getTypeDefinition());
    }

    return false;
}

const ValueTypeContainer *ArrayTypeDefinition::resolveVTC() {
    auto tc = new ValueTypeContainer{.type = T_ARRAY, .vtable = new vtable};

    vec_init(tc->vtable);

    return tc;
}

ScalarTypeDefinition::ScalarTypeDefinition(std::string name, const ValueTypeContainer *vtc)
        : TypeDefinition(vtc), name(std::move(name)) {}

bool ScalarTypeDefinition::isSame(TypeDefinition *other) {
    return TypeDefinition::isSame(other);
}

const ValueTypeContainer *ScalarTypeDefinition::resolveVTC() {
    throw std::logic_error("ScalarTypeDefinition should have a VTC");
}

StructTypeDefinition::StructTypeDefinition(VariablesTable fields) :
        TypeDefinition(), fields(std::move(fields)) {}

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

const ValueTypeContainer *StructTypeDefinition::resolveVTC() {
    auto tc = new ValueTypeContainer{.type = T_OBJECT, .vtable = new vtable};

    vec_init(tc->vtable);

    return tc;
}

InterfaceTypeDefinition::InterfaceTypeDefinition(
        InterfaceTypeDescriptor *descriptor,
        const std::vector<FunctionEntry *> &functions
) : TypeDefinition(), descriptor(descriptor) {
    for (auto function: functions) {
        this->addFunction(
                new ParameterDefinition("i", descriptor, true),
                function
        );
    }
}

bool InterfaceTypeDefinition::canReceiveType(TypeDefinition *type) {
    if (TypeDefinition::canReceiveType(type)) {
        return true;
    }

    for (auto func : functions.entries) {
        if (dynamic_cast<DeclarationFunctionEntry *>(func) != nullptr) {
            auto has = false;

            for (auto otherFunc: type->functions.entries) {
                if (func->isSame(otherFunc)) {
                    has = true;
                    break;
                }
            }

            if (!has) {
                return false;
            }
        }
    }

    return true;
}

const ValueTypeContainer *InterfaceTypeDefinition::resolveVTC() {
    throw std::logic_error("this should not happen");
}

NilTypeDefinition::NilTypeDefinition() : TypeDefinition() {}

bool NilTypeDefinition::isSame(TypeDefinition *other) {
    return true;
}

NilTypeDefinition NilTypeDefinition::Def = NilTypeDefinition();

const ValueTypeContainer *NilTypeDefinition::resolveVTC() {
    return &primitives._nil;
}
