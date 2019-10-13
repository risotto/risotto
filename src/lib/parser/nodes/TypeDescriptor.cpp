//
// Created by rvigee on 10/8/19.
//

#include <lib/compiler/CompilerError.h>

#include <utility>
#include <sstream>
#include "TypeDescriptor.h"
#include "lib/compiler/Compiler.h"

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
    auto fieldRefs = std::vector<StructTypeReference::Field>();

    for (auto ref : fields) {
        fieldRefs.emplace_back(ref.name->lexeme, ref.type->toTypeReference(compiler));
    }

    return new StructTypeReference(fieldRefs);
}

std::string StructTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "struct { ";

    for (auto field:fields) {
        ss << field.name->lexeme << " " << field.type->toString() << "; ";
    }

    ss << " }";

    return ss.str();
}

StructTypeDescriptor::Field::Field(Token * name, TypeDescriptor *type) : name(name), type(type) {}
