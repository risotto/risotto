//
// Created by rvigee on 10/4/19.
//

#include <cassert>
#include "TypesTable.h"
#include "TypeDefinition.h"
#include "lib/parser/nodes/TypeDescriptor.h"

TypeDescriptor *TypesTable::findNamed(const std::string &name) {
    assert(!name.empty());

    for (auto entry : entries) {
        if (auto identifierDesc = dynamic_cast<IdentifierTypeDescriptor *>(entry)) {
            if (identifierDesc->name->lexeme == name) {
                return entry;
            }
        }
    }

    return nullptr;
}

TypeDescriptor *TypesTable::add(TypeDescriptor *typeEntry) {
    return add(typeEntry, true);
}

TypeDescriptor *TypesTable::add(TypeDescriptor *typeEntry, bool allowFindType) {
    if (allowFindType) {
        auto entry = find(typeEntry);
        if (entry) {
            return entry;
        }
    }

    entries.push_back(typeEntry);

    return typeEntry;
}

TypeDescriptor *TypesTable::find(TypeDescriptor *desc) {
    for (auto entry: entries) {
        if (entry->isSame(desc)) {
            return entry;
        }
    }

    return nullptr;
}
