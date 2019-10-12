//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_TYPESTABLE_H
#define RISOTTOV2_TYPESTABLE_H

#include <string>
#include <vector>
#include <map>

class TypeDefinition;

class TypeEntry {
public:
    std::string name;
    TypeDefinition *definition;

    explicit TypeEntry(std::string name, TypeDefinition *definition);
};

class TypesTable {
private:
    std::vector<TypeDefinition *> definitions;
    std::vector<TypeEntry *> entries;
    std::map<std::string, TypeDefinition *> virtualEntries;

public:
    TypeEntry *findNamed(const std::string &name);

    TypeDefinition *add(TypeDefinition *typeDefinition);

    TypeEntry *add(const std::string &name);

    TypeEntry *add(const std::string &name, TypeDefinition *typeDefinition);

    TypeDefinition *findVirtual(const std::string &name);

    TypeDefinition *addVirtual(const std::string &id, TypeDefinition *typeDefinition);
};


#endif //RISOTTOV2_TYPESTABLE_H
