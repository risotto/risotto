//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"

#include <utility>
#include <sstream>
#include <lib/compiler/utils/Utils.h>
#include "TypeDefinition.h"
#include "Compiler.h"

NamedTypeReference::NamedTypeReference(std::string name, TypeDefinition *entry) : ConcreteTypeReference(entry),
                                                                                  name(std::move(name)) {}

std::string NamedTypeReference::toString() {
    return name;
}

ArrayTypeReference::ArrayTypeReference(TypeReference *element) : element(element) {

}

FunctionEntry *ArrayTypeReference::findOperator(Frame *frame, const std::string &name,
                                                const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

FunctionEntry *
ArrayTypeReference::findPrefix(Frame *frame, const std::string &name, const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

bool ArrayTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherArray = dynamic_cast<ArrayTypeReference *>(other)) {
        return element->canReceiveType(otherArray->element);
    }

    return false;
}

std::string ArrayTypeReference::toString() {
    return "[]" + element->toString();
}

TypeDefinition *ArrayTypeReference::toTypeDefinition(Frame *frame) {
    return new ArrayTypeDefinition(element->toTypeDefinition(frame));
}

std::vector<FunctionEntry *> ArrayTypeReference::findFunctionsCandidates(Frame *frame, const std::string &name) {
    auto def = frame->findVirtualType(this);

    if (def == nullptr) {
        return std::vector<FunctionEntry *>();
    }

    return def->functions.findCandidates(name);
}

FunctionTypeReference *TypeReference::asFunction() {
    return nullptr;
}


bool FunctionTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherFunction = other->asFunction()) {
        return Utils::typesMatch(params, otherFunction->params);
    }

    return false;
}

FunctionTypeReference *FunctionTypeReference::asFunction() {
    return this;
}

std::string FunctionTypeReference::toString() {
    return "func () ... { ... }";
}

FunctionTypeReference::FunctionTypeReference(std::vector<FunctionTypeReferenceParameter> params,
                                             std::vector<TypeReference *> returnTypes) : params(std::move(params)),
                                                                                         returnTypes(std::move(returnTypes)) {}

TypeDefinition *FunctionTypeReference::toTypeDefinition(Frame *frame) {
    throw std::logic_error("Unimplemented");
}

ConcreteTypeReference::ConcreteTypeReference(TypeDefinition *entry) : definition(entry) {
}

FunctionEntry *ConcreteTypeReference::findOperator(Frame *frame, const std::string &name,
                                                   const std::vector<TypeReference *> &types) {
    return definition->operators.find(name, types);
}

FunctionEntry *ConcreteTypeReference::findPrefix(Frame *frame, const std::string &name,
                                                 const std::vector<TypeReference *> &types) {
    return definition->prefixes.find(name, types);
}

bool ConcreteTypeReference::canReceiveType(TypeReference *other) {
    if (auto concrete = dynamic_cast<ConcreteTypeReference *>(other)) {
        return concrete->definition == definition;
    }

    return false;
}

std::string ConcreteTypeReference::toString() {
    if (auto concrete = dynamic_cast<ConcreteTypeDefinition *>(definition)) {
        return concrete->name;
    }

    std::stringstream ss;
    ss << definition;

    return ss.str();
}

TypeDefinition *ConcreteTypeReference::toTypeDefinition(Frame *frame) {
    return definition;
}

std::vector<FunctionEntry *>
ConcreteTypeReference::findFunctionsCandidates(Frame *frame, const std::string &name) {
    return definition->functions.findCandidates(name);
}

bool StructTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherStruct = dynamic_cast<StructTypeReference *>(other)) {
        for (auto field:definition->fields) {
            auto has = false;

            for (const auto &otherField : otherStruct->definition->fields) {
                if (field->name == otherField->name) {
                    if (field->typeRef->canReceiveType(otherField->typeRef)) {
                        has = true;
                        break;
                    }
                }
            }

            if (!has) {
                return false;
            }
        }
    }

    if (auto otherConcrete = dynamic_cast<ConcreteTypeReference *>(other)) {
        return definition == otherConcrete->definition;
    }

    return false;
}

std::string StructTypeReference::toString() {
    std::stringstream ss;

    ss << "struct { ";

    for (const auto &field : definition->fields) {
        ss << field->name << "; ";
    }

    ss << "}";

    return ss.str();
}

TypeDefinition *StructTypeReference::toTypeDefinition(Frame *frame) {
    return definition;
}

StructTypeReference::StructTypeReference(StructTypeDefinition *entry) : definition(entry) {}

FunctionTypeReferenceParameter::FunctionTypeReferenceParameter(std::string name, TypeReference *type,
                                                               bool asReference) : name(std::move(name)), type(type),
                                                                                   asReference(asReference) {}
