//
// Created by rvigee on 10/8/19.
//

#include <lib/compiler/CompilerError.h>
#include "TypeDescriptor.h"
#include "lib/compiler/Compiler.h"

TypeDescriptor::TypeDescriptor(Token *name, bool isArray) : name(name), isArray(isArray) {}

TypeReference TypeDescriptor::toTypeReference(Compiler *compiler) {
    auto paramType = compiler->frame->types.find(name->lexeme);

    if (paramType == nullptr) {
        throw CompilerError("Cannot find type for " + name->lexeme);
    }

    return TypeReference(paramType, isArray);
}
