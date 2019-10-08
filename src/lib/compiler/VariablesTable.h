//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_VARIABLESTABLE_H
#define RISOTTOV2_VARIABLESTABLE_H

#include <string>
#include "TypesTable.h"

class TypeReference;

class VariableEntry {
public:
    std::string name;
    TypeReference * typeRef;
    int index;

    VariableEntry(std::string name, TypeReference * typeRef, int index);
};

class VariablesTable {
private:
    std::vector<VariableEntry *> entries;
public:
    VariableEntry *find(const std::string &name);

    VariableEntry *add(std::string name, TypeReference * typeRef);
};


#endif //RISOTTOV2_VARIABLESTABLE_H
