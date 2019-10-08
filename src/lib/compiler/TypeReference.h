//
// Created by rvigee on 10/7/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H

#include <vector>
#include <string>

class TypeEntry;
class FunctionEntry;

class TypeReference {
public:
    TypeEntry *entry;
    bool isArray;

    TypeReference(TypeEntry *entry, bool isArray);

    FunctionEntry *findOperator(const std::string& name, const std::vector<TypeReference>& types);
    FunctionEntry *findPrefix(const std::string& name, const std::vector<TypeReference>& types);

    bool canReceiveType(TypeReference other);
    bool isFunction();
};

class TypeReferences: public std::vector<TypeReference> {
public:
    using std::vector<TypeReference>::vector;

    TypeReferences(TypeEntry *entry);
    TypeReferences(TypeReference ref);

    bool single();

    TypeReferences onlyFunctions();
};

#endif //RISOTTOV2_TYPEREFERENCE_H
