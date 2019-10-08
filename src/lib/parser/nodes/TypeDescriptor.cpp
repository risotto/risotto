//
// Created by rvigee on 10/8/19.
//

#include <lib/compiler/CompilerError.h>
#include "TypeDescriptor.h"
#include "lib/compiler/Compiler.h"

ConcreteTypeDescriptor::ConcreteTypeDescriptor(Token *name) : name(name) {}

TypeReference *ConcreteTypeDescriptor::toTypeReference(Compiler *compiler) {
    auto paramType = compiler->frame->types.find(name->lexeme);

    if (paramType == nullptr) {
        throw CompilerError("Cannot find type for " + name->lexeme);
    }

    return new ConcreteTypeReference(paramType);
}

std::string ConcreteTypeDescriptor::toString() {
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
