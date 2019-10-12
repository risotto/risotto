//
// Created by rvigee on 10/8/19.
//

#include <lib/compiler/CompilerError.h>

#include <utility>
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

//FunctionTypeDescriptor::FunctionTypeDescriptor(std::vector<TypeDescriptor *> parameters, TypeDescriptor *returnType): parameters(std::move(parameters)), returnType(returnType) {
//
//}

InterfaceTypeDescriptor::InterfaceTypeDescriptor(std::vector<Function *> functions): functions(std::move(functions)) {

}

std::string InterfaceTypeDescriptor::toString() {
    return "interface { ... }";
}

TypeReference *InterfaceTypeDescriptor::toTypeReference(Compiler *compiler) {
    // TODO: implement interface functions
    return new InterfaceTypeReference({});
}

InterfaceTypeDescriptor::Function::Function(std::string name, std::vector<TypeDescriptor *> arguments,
                                            TypeDescriptor *returnType) : name(std::move(name)), arguments(std::move(arguments)),
                                                                          returnType(returnType) {}
