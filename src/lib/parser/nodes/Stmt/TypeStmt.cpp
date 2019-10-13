//
// Created by rvigee on 10/13/19.
//

#include <lib/compiler/Compiler.h>
#include "TypeStmt.h"

TypeStmt::TypeStmt(Token *name, TypeDescriptor *typeDescriptor) : name(name), typeDescriptor(typeDescriptor) {}

std::vector<ByteResolver *> TypeStmt::compile(Compiler *compiler) {
    compiler->frame->types.add(name->lexeme, typeDescriptor->toTypeReference(compiler)->toTypeDefinition(compiler));

    return std::vector<ByteResolver *>();
}

