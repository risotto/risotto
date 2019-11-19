//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_TYPESTABLE_H
#define RISOTTOV2_TYPESTABLE_H

#include <string>
#include <vector>
#include <map>

class TypeDefinition;

class TypeDescriptor;

class TypeEntry {
public:
    std::string name;
    TypeDefinition *definition;

    explicit TypeEntry(TypeDefinition *definition);

    TypeEntry(std::string name, TypeDefinition *definition);
};

class TypesTable {
public:
    std::vector<TypeEntry *> entries;

    TypeEntry *findNamed(const std::string &name);

    TypeEntry *add(TypeEntry *typeEntry);

    TypeEntry *reg(const std::string &name);

    TypeEntry *add(const std::string &name, TypeDefinition *typeDefinition);
};

#endif //RISOTTOV2_TYPESTABLE_H
