//
// Created by rvigee on 10/4/19.
//

#include "TypeReference.h"
#include "Compiler.h"

TypeEntry *TypeReference::getType(Compiler *c) {
    if (typeEntry) {
        return typeEntry;
    }

    typeEntry = c->frame->findType(typeName);

    return typeEntry;
}
