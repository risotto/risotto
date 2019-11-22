//
// Created by rvigee on 10/13/19.
//

#include <lib/compiler/Compiler.h>
#include "TypeStmt.h"

TypeStmt::TypeStmt(Token *name, TypeDescriptor *typeDescriptor) : name(name), typeDescriptor(typeDescriptor) {}

std::vector<ByteResolver *> TypeStmt::compile(Compiler *compiler) {
    typeEntry->definition = typeDescriptor->getTypeDefinition();

    return std::vector<ByteResolver *>();
}

void TypeStmt::symbolize(Compiler *compiler) {
    typeDescriptor->createLinkUnits(compiler->linkables, compiler->frame);

    typeEntry = compiler->frame->types.reg(name->lexeme);
}

