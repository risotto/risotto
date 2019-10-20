//
// Created by rvigee on 10/8/19.
//

#include <lib/compiler/CompilerError.h>

#include <cassert>
#include <utility>
#include <sstream>
#include "TypeDescriptor.h"
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeDefinition.h"

IdentifierTypeDescriptor::IdentifierTypeDescriptor(Token *name) : name(name) {}

TypeReference *IdentifierTypeDescriptor::toTypeReference(Compiler *compiler) {
    auto paramType = compiler->frame->findNamedType(name->lexeme);

    if (paramType == nullptr) {
        throw CompilerError("Cannot find type for " + name->lexeme);
    }

    return new NamedTypeReference(name->lexeme, paramType);
}

std::string IdentifierTypeDescriptor::toString() {
    return name->lexeme;
}

ArrayTypeDescriptor::ArrayTypeDescriptor(TypeDescriptor *element) : element(element) {

}

TypeReference *ArrayTypeDescriptor::toTypeReference(Compiler *compiler) {
    return new ArrayTypeReference(element->toTypeReference(compiler));
}

std::string ArrayTypeDescriptor::toString() {
    return "[]" + element->toString();
}

StructTypeDescriptor::StructTypeDescriptor(std::vector<Field> fields) : fields(std::move(fields)) {}

TypeReference *StructTypeDescriptor::toTypeReference(Compiler *compiler) {
    if (typeDefinition == nullptr) {
        auto fieldDefs = VariablesTable();
        for (auto field : fields) {
            auto fieldTypeRef = field.type->toTypeReference(compiler);
            fieldDefs.add(field.name->lexeme, fieldTypeRef);
        }
        auto typeDef = new StructTypeDefinition(fieldDefs);

        typeDefinition = dynamic_cast<StructTypeDefinition *>(compiler->frame->types.add(typeDef));
    }

    return new StructTypeReference(typeDefinition);
}

std::string StructTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "struct { ";

    for (auto field:fields) {
        ss << field.name->lexeme << " " << field.type->toString() << "; ";
    }

    ss << "}";

    return ss.str();
}

StructTypeDescriptor::Field::Field(Token *name, TypeDescriptor *type) : name(name), type(type) {}

FunctionTypeDescriptor::FunctionTypeDescriptor(std::vector<ParameterDefinition> params,
                                               std::vector<TypeDescriptor *> returnTypes)
        : params(std::move(params)), returnTypes(std::move(returnTypes)) {}

TypeReference *FunctionTypeDescriptor::toTypeReference(Compiler *compiler) {
    auto paramsRefs = std::vector<FunctionTypeReferenceParameter>();
    for (auto param: params) {
        paramsRefs.emplace_back(param.name->lexeme, param.type->toTypeReference(compiler), param.asReference);
    }

    auto returnTypesRefs = std::vector<TypeReference *>();
    for (auto returnType: returnTypes) {
        returnTypesRefs.push_back(returnType->toTypeReference(compiler));
    }

    return new FunctionTypeReference(paramsRefs, returnTypesRefs);
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

InterfaceTypeDescriptor::InterfaceTypeDescriptor(std::vector<std::pair<Token *, FunctionTypeDescriptor *>> functions) :
        functions(std::move(functions)) {}

TypeReference *InterfaceTypeDescriptor::toTypeReference(Compiler *compiler) {
    auto functionRefs = std::vector<std::pair<std::string, FunctionTypeReference *>>();

    for (auto entry: functions) {
        auto ref = dynamic_cast<FunctionTypeReference *>(entry.second->toTypeReference(compiler));
        assert(ref != nullptr);

        functionRefs.emplace_back(entry.first->lexeme, ref);
    }

    return new InterfaceTypeReference(functionRefs);
}

std::string InterfaceTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "interface { ";

    for (auto function:functions) {
        ss << function.first->lexeme << ": " << function.second->toString() << " ";
    }

    ss << "}";

    return ss.str();
}
