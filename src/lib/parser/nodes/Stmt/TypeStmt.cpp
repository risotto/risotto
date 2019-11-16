//
// Created by rvigee on 10/13/19.
//

#include <lib/compiler/Compiler.h>
#include "TypeStmt.h"

TypeStmt::TypeStmt(Token *name, TypeDescriptor *typeDescriptor) : name(name), typeDescriptor(typeDescriptor) {}

std::vector<ByteResolver *> TypeStmt::compile(Compiler *compiler) {
    auto def = typeDescriptor->toTypeReference(compiler)->getTypeDefinition(compiler);
    compiler->frame->types.add(name->lexeme, def);

    compiler->typesManager.registerType(def);

    return std::vector<ByteResolver *>();
}

