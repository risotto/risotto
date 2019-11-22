//
// Created by rvigee on 10/13/19.
//

#include <lib/compiler/Compiler.h>
#include "TypeStmt.h"

TypeStmt::TypeStmt(Token *name, TypeDescriptor *typeDescriptor) : name(name), typeDescriptor(typeDescriptor) {}

std::vector<ByteResolver *> TypeStmt::compile(Compiler *compiler) {
    return {};
}

void TypeStmt::symbolize(Compiler *compiler) {
    TypeDescriptor *identifierDesc = new IdentifierTypeDescriptor(name, [this](Frame *frame){
        return typeDescriptor->genType(frame);
    });
    compiler->typesManager->add(typeDescriptor, compiler->frame);
    compiler->typesManager->add(identifierDesc, compiler->frame);
}
