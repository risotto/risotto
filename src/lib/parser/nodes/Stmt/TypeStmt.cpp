//
// Created by rvigee on 10/13/19.
//

#include <lib/compiler/Compiler.h>
#include "TypeStmt.h"

TypeStmt::TypeStmt(PToken name, TypeDescriptor *typeDescriptor) : name(name), typeDescriptor(typeDescriptor) {}

std::vector<ByteResolver *> TypeStmt::compile(Compiler *compiler) {
    return {};
}

void TypeStmt::symbolize(Compiler *compiler) {
    auto desc = new IdentifierTypeDescriptor(name, typeDescriptor);

    compiler->frame->types.add(desc, false);
    compiler->typesManager->createLinkUnits(typeDescriptor, compiler->frame);

    compiler->typesManager->addListener([this, desc](){
        desc->typeDef = typeDescriptor->getTypeDefinition();

        return desc->typeDef != nullptr;
    });
}
