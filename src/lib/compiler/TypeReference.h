//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>

class TypeEntry;

class TypeReferences: public std::vector<TypeEntry *> {
public:
    using std::vector<TypeEntry *>::vector;

    TypeReferences(TypeEntry *entry);

    bool single();

    TypeReferences onlyFunctions();
};

class TypeReference {
public:
    TypeEntry *type;
    bool isArray;

    TypeReference(TypeEntry *type, bool isArray);
};

#endif //RISOTTOV2_TYPEREFERENCE_H
