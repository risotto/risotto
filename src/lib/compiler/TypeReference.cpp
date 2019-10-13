//
// Created by rvigee on 10/7/19.
//

#include "lib/compiler/TypeReference.h"

#include <utility>
#include <sstream>
#include "TypeDefinition.h"
#include "Compiler.h"

NamedTypeReference::NamedTypeReference(std::string name, TypeDefinition *entry) : ConcreteTypeReference(entry),
                                                                                  name(std::move(name)) {}

std::string NamedTypeReference::toString() {
    return name;
}

ArrayTypeReference::ArrayTypeReference(TypeReference *element) : element(element) {

}

FunctionEntry *ArrayTypeReference::findOperator(Compiler *compiler, const std::string &name,
                                                const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

FunctionEntry *
ArrayTypeReference::findPrefix(Compiler *compiler, const std::string &name, const std::vector<TypeReference *> &types) {
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

FunctionEntry *ArrayTypeReference::findFunction(Compiler *compiler, const std::string &name,
                                                const std::vector<TypeReference *> &types) {
    throw std::logic_error("Unimplemented");
}

TypeDefinition *ArrayTypeReference::toTypeDefinition(Compiler *compiler) {
    return new ArrayTypeDefinition(element->toTypeDefinition(compiler));
}

bool TypeReference::isFunction() {
    return false;
}

FunctionTypeReference::FunctionTypeReference(FunctionTypeDefinition *entry) : entry(entry) {

}

bool FunctionTypeReference::canReceiveType(TypeReference *other) {
    throw std::logic_error("Unimplemented");

}

bool FunctionTypeReference::isFunction() {
    return true;
}

std::string FunctionTypeReference::toString() {
    return "func () ... { ... }";
}

TypeDefinition *FunctionTypeReference::toTypeDefinition(Compiler *compiler) {
    return entry;
}

ConcreteTypeReference::ConcreteTypeReference(TypeDefinition *entry) : entry(entry) {
    if (dynamic_cast<ConcreteTypeDefinition *>(entry) == nullptr) {
        throw std::logic_error("entry should be ConcreteTypeDefinition");
    }
}

FunctionEntry *ConcreteTypeReference::findFunction(Compiler *compiler, const std::string &name,
                                                   const std::vector<TypeReference *> &types) {
    return entry->functions.find(name, types);
}

FunctionEntry *ConcreteTypeReference::findOperator(Compiler *compiler, const std::string &name,
                                                   const std::vector<TypeReference *> &types) {
    return entry->operators.find(name, types);
}

FunctionEntry *ConcreteTypeReference::findPrefix(Compiler *compiler, const std::string &name,
                                                 const std::vector<TypeReference *> &types) {
    return entry->prefixes.find(name, types);
}

bool ConcreteTypeReference::canReceiveType(TypeReference *other) {
    if (auto concrete = dynamic_cast<ConcreteTypeReference *>(other)) {
        return concrete->entry == entry;
    }

    if (auto named = dynamic_cast<NamedTypeReference *>(other)) {
        return named->entry == entry;
    }

    return false;
}

std::string ConcreteTypeReference::toString() {
    if (auto concrete = dynamic_cast<ConcreteTypeDefinition *>(entry)) {
        return concrete->name;
    }

    std::stringstream ss;
    ss << entry;

    return ss.str();
}

TypeDefinition *ConcreteTypeReference::toTypeDefinition(Compiler *compiler) {
    return entry;
}

StructTypeReference::Field::Field(std::string name, TypeReference *type) : name(std::move(name)), type(type) {}

StructTypeReference::StructTypeReference(std::vector<Field> fields) : fields(std::move(fields)) {}

bool StructTypeReference::canReceiveType(TypeReference *other) {
    if (auto otherStruct = dynamic_cast<StructTypeReference *>(other)) {
        for (auto field:fields) {
            auto has = false;

            for (const auto& otherField : otherStruct->fields) {
                if (field.name == otherField.name) {
                    if (field.type->canReceiveType(otherField.type)) {
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

    return false;
}

std::string StructTypeReference::toString() {
    std::stringstream ss;

    ss << "struct { ";

    for (auto field:fields) {
        ss << field.name << " " << field.type->toString() << "; ";
    }

    ss << "}";

    return ss.str();
}

TypeDefinition *StructTypeReference::toTypeDefinition(Compiler *compiler) {
    return nullptr;
}
