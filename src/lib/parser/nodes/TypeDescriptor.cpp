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
    this->typeDef = typeDef;
}

IdentifierTypeDescriptor::IdentifierTypeDescriptor(Token *name,
                                                   std::function<TypeDefinition *(Frame *frame)> typeDefGen)
        : IdentifierTypeDescriptor(name) {
    this->typeDefGen = std::move(typeDefGen);
}

TypeDefinition *IdentifierTypeDescriptor::genType(Frame *frame) {
    if (typeDefGen) {
        return typeDefGen(frame);
    }

    auto desc = frame->findNamedType(name->lexeme);

    return desc->getTypeDefinition();
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

void IdentifierTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {

}

ArrayTypeDescriptor::ArrayTypeDescriptor(TypeDescriptor *element) : element(element) {

}

std::string ArrayTypeDescriptor::toString() {
    return "[]" + element->toString();
}

TypeDefinition *ArrayTypeDescriptor::genType(Frame *frame) {
    return new ArrayTypeDefinition(element);
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

void ArrayTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    typesManager->add(element, frame);
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

TypeDefinition *StructTypeDescriptor::genType(Frame *frame) {
    auto structFields = VariablesTable();
    for (auto field: fields) {
        structFields.add(field.name->lexeme, field.type);
    }

    return new StructTypeDefinition(structFields);
}

void StructTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    for (auto field: fields) {
        typesManager->add(field.type, frame);
    }
}

bool StructTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto structDef = dynamic_cast<StructTypeDescriptor *>(type)) {
        if (structDef->fields.size() == fields.size()) {
            for (int i = 0; i < fields.size(); ++i) {
                auto field = fields[i];
                auto otherField = structDef->fields[i];

                if (!field.type->isSame(otherField.type)) {
                    return false;
                }
            }
        }
    }

    return false;
}

StructTypeDescriptor::Field::Field(Token *name, TypeDescriptor *type) : name(name), type(type) {}

FunctionTypeDescriptor::FunctionTypeDescriptor(std::vector<ParameterDefinition *> params,
                                               std::vector<TypeDescriptor *> returnTypes)
        : params(std::move(params)), returnTypes(std::move(returnTypes)) {}

std::string FunctionTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "func (";

    for (int i = 0; i < params.size(); ++i) {
        auto param = params[i];

        if (i != 0) {
            ss << ", ";
        }

        ss << param->name->lexeme << " " << param->type->toString();
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

TypeDefinition *FunctionTypeDescriptor::genType(Frame *frame) {
    return new FunctionTypeDefinition(this);
}

void FunctionTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    for (auto param: params) {
        typesManager->add(param->type, frame);
    }

    for (auto returnType: returnTypes) {
        typesManager->add(returnType, frame);
    }
}

bool FunctionTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto functionType = dynamic_cast<FunctionTypeDescriptor *>(type)) {
        if (params.size() != functionType->params.size()) {
            return false;
        }

        if (returnTypes.size() != functionType->returnTypes.size()) {
            return false;
        }

        for (int i = 0; i < params.size(); ++i) {
            auto param = params[i];
            auto functionParam = functionType->params[i];

            if (!param->type->isSame(functionParam->type)) {
                return false;
            }
        }

        for (int i = 0; i < returnTypes.size(); ++i) {
            auto returnType = returnTypes[i];
            auto functionReturnType = functionType->returnTypes[i];

            if (!returnType->isSame(functionReturnType)) {
                return false;
            }
        }

        return true;
    }

    return false;
}

TypeDefinition *TypeDescriptor::getTypeDefinition() {
    return typeDef;
}

bool TypeDescriptor::canReceiveType(TypeDescriptor *type) {
    auto typeDef = getTypeDefinition();
    auto otherTypeDef = type->getTypeDefinition();

    assert(typeDef != nullptr);
    assert(otherTypeDef != nullptr);

    return typeDef->canReceiveType(otherTypeDef);
}

bool TypeDescriptor::isSame(TypeDescriptor *type) {
    return this == type;
}

bool TypeDescriptor::resolveType(Frame *frame, bool allowFindType) {
    if (typeDef != nullptr) {
        return true;
    }

    if (allowFindType) {
        if (auto desc = frame->types.find(this)) {
            typeDef = desc->getTypeDefinition();
        } else {
            typeDef = genType(frame);
        }
    } else {
        typeDef = genType(frame);
    }

    auto entry = frame->types.add(this, allowFindType);
    this->typeDef = entry->typeDef;

    return typeDef != nullptr;
}
