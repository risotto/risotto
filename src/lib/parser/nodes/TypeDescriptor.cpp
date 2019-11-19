//
// Created by rvigee on 10/8/19.
//

#include "TypeDescriptor.h"
#include <lib/compiler/CompilerError.h>
#include <utility>
#include <sstream>
#include <cassert>
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeDefinition.h"

std::string IdentifierTypeDescriptor::toString() {
    return name->lexeme;
}

IdentifierTypeDescriptor::IdentifierTypeDescriptor(Token *name) : name(name) {
}

IdentifierTypeDescriptor::IdentifierTypeDescriptor(const std::string &name, TypeDefinition *typeDef) :
        IdentifierTypeDescriptor(Token::IdentifierFactory(name)) {
    typeEntry = new TypeEntry(name, typeDef);
}

TypeEntry *IdentifierTypeDescriptor::genType(Frame *frame) {
    auto entry = frame->findNamedType(name->lexeme);

    return entry;
}

bool IdentifierTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto otherIdentifier = dynamic_cast<IdentifierTypeDescriptor *>(type)) {
        return name->lexeme == otherIdentifier->name->lexeme;
    }

    return false;
}

ArrayTypeDescriptor::ArrayTypeDescriptor(TypeDescriptor *element) : element(element) {

}

std::string ArrayTypeDescriptor::toString() {
    return "[]" + element->toString();
}

TypeEntry *ArrayTypeDescriptor::genType(Frame *frame) {
    for (auto entry: frame->types.entries) {
        if (auto arrayDef = dynamic_cast<ArrayTypeDefinition *>(entry->definition)) {
            if (element->isSame(arrayDef->element)) {
                return entry;
            }
        }
    }

    auto elementEntry = element->genType(frame);
    if (elementEntry == nullptr) {
        return nullptr;
    }
    element->setTypeEntry(elementEntry);

    auto entry = new TypeEntry(new ArrayTypeDefinition(element));

    frame->types.add(entry);

    return entry;
}

bool ArrayTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto otherArray = dynamic_cast<ArrayTypeDescriptor *>(type)) {
        return element->isSame(otherArray->element);
    }

    return false;
}

StructTypeDescriptor::StructTypeDescriptor(std::vector<Field> fields) : fields(std::move(fields)) {}

std::string StructTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "struct { ";

    for (auto field:fields) {
        ss << field.name->lexeme << " " << field.type->toString() << "; ";
    }

    ss << "}";

    return ss.str();
}

TypeEntry *StructTypeDescriptor::genType(Frame *frame) {
    for (auto entry: frame->types.entries) {
        if (auto structDef = dynamic_cast<StructTypeDefinition *>(entry->definition)) {
            if (structDef->fields.size() == fields.size()) {
                return entry;
            }
        }
    }

    auto structFields = VariablesTable();
    for (auto field: fields) {
        auto fieldEntry = field.type->genType(frame);
        if (fieldEntry == nullptr) {
            return nullptr;
        }
        field.type->setTypeEntry(fieldEntry);

        structFields.add(field.name->lexeme, field.type);
    }

    auto entry = new TypeEntry(new StructTypeDefinition(structFields));

    frame->types.add(entry);

    return entry;
}

StructTypeDescriptor::Field::Field(Token *name, TypeDescriptor *type) : name(name), type(type) {}

FunctionTypeDescriptor::FunctionTypeDescriptor(std::vector<ParameterDefinition> params,
                                               std::vector<TypeDescriptor *> returnTypes)
        : params(std::move(params)), returnTypes(std::move(returnTypes)) {}

FunctionTypeDescriptor::FunctionTypeDescriptor(FunctionEntry *functionEntry) : FunctionTypeDescriptor(
        functionEntry->params, functionEntry->returnTypes) {
    typeEntry = new TypeEntry(functionEntry->typeDefinition);
}

std::string FunctionTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "func (";

    for (int i = 0; i < params.size(); ++i) {
        auto param = params[i];

        if (i != 0) {
            ss << ", ";
        }

        ss << param.name->lexeme << " " << param.type->toString();
    }

    ss << ")";

    if (!returnTypes.empty()) {
        ss << " (";
        for (int i = 0; i < returnTypes.size(); ++i) {
            auto type = returnTypes[i];

            if (i != 0) {
                ss << ", ";
            }

            ss << type->toString();
        }
        ss << ")";
    }

    return ss.str();
}

TypeEntry *FunctionTypeDescriptor::genType(Frame *frame) {
    for (auto param: params) {
        auto entry = param.type->genType(frame);
        if (entry == nullptr) {
            return nullptr;
        }

        param.type->setTypeEntry(entry);
    }

    auto functionEntry = new FunctionEntry("", params, returnTypes);
    auto typeDef = new FunctionTypeDefinition(functionEntry);
    auto entry = new TypeEntry(typeDef);

    frame->types.add(entry);
}

TypeDefinition *TypeDescriptor::getTypeDefinition() {
    assert(typeEntry != nullptr);

    return typeEntry->definition;
}

bool TypeDescriptor::canReceiveType(TypeDescriptor *type) {
    return getTypeDefinition()->canReceiveType(type->getTypeDefinition());
}

void TypeDescriptor::setTypeEntry(TypeEntry *entry) {
    assert(entry != nullptr);
    assert(typeEntry == nullptr);

    typeEntry = entry;
}

bool TypeDescriptor::isSame(TypeDescriptor *type) {
    return this == type;
}
