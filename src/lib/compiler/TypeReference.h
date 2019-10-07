//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>

class TypeEntry;

class TypesEntries: public std::vector<TypeEntry *> {
public:
    using std::vector<TypeEntry *>::vector;

    TypesEntries(TypeEntry *entry);

    bool single();

    TypesEntries onlyFunctions();
};

class TypeReference {
public:
    TypeEntry *type;
    bool isArray;

    TypeReference(TypeEntry *type, bool isArray);
};

#endif //RISOTTOV2_TYPEREFERENCE_H
