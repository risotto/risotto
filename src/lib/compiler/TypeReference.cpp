//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"

#include <cassert>
#include <utility>
#include <sstream>
#include <lib/compiler/utils/Utils.h>
#include "TypeDefinition.h"
#include "Compiler.h"

static TypeDefinition *
virtualTypeHandler(Compiler *compiler, TypeReference *ref, const std::function<TypeDefinition *()> &f) {
    auto def = compiler->frame->findVirtualType(ref);

    if (def) {
        return def;
    }

    return compiler->frame->types.addVirtual(ref->toString(), f());
}

TypeReference::TypeReference() : TypeReference(nullptr) {}

TypeReference::TypeReference(TypeDefinition *typeDefinition) : typeDefinition(typeDefinition) {}

TypeDefinition *TypeReference::getTypeDefinition() {
    return getTypeDefinition(nullptr);
}

TypeDefinition *TypeReference::getTypeDefinition(Compiler *compiler) {
    if (typeDefinition != nullptr) {
        return typeDefinition;
    }

    assert(compiler != nullptr);

    typeDefinition = computeTypeDefinition(compiler);

    return typeDefinition;
}

ArrayTypeReference::ArrayTypeReference(TypeReference *element) : TypeReference(), element(element) {}

std::string ArrayTypeReference::toString() {
    return "[]" + element->toString();
}

TypeDefinition *ArrayTypeReference::computeTypeDefinition(Compiler *compiler) {
    return virtualTypeHandler(compiler, this, [this, compiler]() {
        return new ArrayTypeDefinition(element->getTypeDefinition(compiler));
    });
}

bool ArrayTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherArray = dynamic_cast<ArrayTypeReference *>(other)) {
        return element->canReceiveType(otherArray->element);
    }

    return false;
}

FunctionTypeReferenceParameter::FunctionTypeReferenceParameter(
        std::string name,
        TypeReference *type,
        bool asReference
) : name(std::move(name)), type(type), asReference(asReference) {}

FunctionTypeReference::FunctionTypeReference(
        FunctionEntry *entry
) : TypeReference(entry->typeDefinition) {
    params = std::vector<FunctionTypeReferenceParameter>();

    for (const auto &param: entry->params) {
        params.emplace_back(param.name, param.type, param.asReference);
    }

    returnTypes = entry->returnTypes;
}

FunctionTypeReference::FunctionTypeReference(
        std::vector<FunctionTypeReferenceParameter> params,
        std::vector<TypeReference *> returnTypes
) : params(std::move(params)), returnTypes(std::move(returnTypes)) {}

std::string FunctionTypeReference::toString() {
    std::stringstream ss;

    ss << "func (";

    for (int i = 0; i < params.size(); ++i) {
        auto param = params[i];

        if (i > 0) {
            ss << ", ";
        }

        ss << param.name << " " << param.type->toString();
    }

    ss << ")";

    return ss.str();
}

TypeDefinition *FunctionTypeReference::computeTypeDefinition(Compiler *compiler) {
    throw std::logic_error("Functions must have a type definition");
}

bool FunctionTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherFunction = dynamic_cast<FunctionTypeReference *>(other)) {
        return Utils::typesMatch(params, otherFunction->params);
    }

    return false;
}

NamedTypeReference::NamedTypeReference(std::string name, TypeDefinition *def) :
        TypeReference(def), name(std::move(name)) {}

std::string NamedTypeReference::toString() {
    return name;
}

TypeDefinition *NamedTypeReference::computeTypeDefinition(Compiler *compiler) {
    return compiler->frame->findNamedType(name);
}

bool NamedTypeReference::canReceiveType(TypeReference *other) {
    return getTypeDefinition() == other->getTypeDefinition();
}

StructTypeReference::StructTypeReference(StructTypeDefinition *def) : TypeReference(def) {}

std::string StructTypeReference::toString() {
    std::stringstream ss;

    ss << "struct {";

    ss << " ... ";

    ss << "}";

    return ss.str();
}

TypeDefinition *StructTypeReference::computeTypeDefinition(Compiler *compiler) {
    throw std::logic_error("Structs must have a type definition");
}

bool StructTypeReference::canReceiveType(TypeReference *other) {
    return getTypeDefinition() == other->getTypeDefinition();
}

InterfaceTypeReference::InterfaceTypeReference(
        std::vector<std::pair<std::string, FunctionTypeReference *>> functions
) : TypeReference(), functions(std::move(functions)) {}

std::string InterfaceTypeReference::toString() {
    std::stringstream ss;

    ss << "interface {";

    for (auto entry: functions) {
        ss << entry.first << ": " << entry.second->toString() << "; ";
    }

    ss << "}";

    return ss.str();
}

TypeDefinition *InterfaceTypeReference::computeTypeDefinition(Compiler *compiler) {
    return virtualTypeHandler(compiler, this, [this]() {
        auto def = new InterfaceTypeDefinition();

        for (const auto &entry: functions) {
            auto params = std::vector<FunctionEntryParameter>();

            for (const auto &param: entry.second->params) {
                params.emplace_back(param.name, param.type, param.asReference);
            }

            def->addFunction(
                    entry.first,
                    false,
                    new InterfaceFunctionEntry(
                            entry.first,
                            params,
                            entry.second->returnTypes
                    )
            );
        }

        return def;
    });
}

bool InterfaceTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherNamed = dynamic_cast<NamedTypeReference *>(other)) {
        other = otherNamed->getTypeDefinition()->getTypeReference();
    }

    if (auto otherInterface = dynamic_cast<InterfaceTypeReference *>(other)) {
        for (auto pair: functions) {
            auto has = false;
            for (const auto &otherPair: otherInterface->functions) {
                if (pair.first == otherPair.first) {
                    if (pair.second->canReceiveType(otherPair.second)) {
                        has = true;
                        break;
                    }
                }
            }

            if (!has) {
                return false;
            }
        }

        return true;
    }

    if (auto otherStruct = dynamic_cast<StructTypeReference *>(other)) {
        for (const auto &pair: functions) {
            auto paramsTypes = std::vector<TypeReference *>();
            for (const auto &param: pair.second->params) {
                paramsTypes.push_back(param.type);
            }

            if (!otherStruct->getTypeDefinition()->functions.find(pair.first, paramsTypes)) {
                return false;
            }
        }

        return true;
    }

    return false;
}
