//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_VARIABLESTABLE_H
#define RISOTTOV2_VARIABLESTABLE_H

#include <string>
#include "TypesTable.h"

class VariableEntry {
public:
    std::string name;
    TypeEntry *type;
    int index;

    VariableEntry(std::string name, TypeEntry *typeEntry, int index);
};

class VariablesTable {
private:
    std::vector<VariableEntry *> variables;
public:
    VariableEntry *find(const std::string &name);

    VariableEntry *add(std::string name, TypeEntry *typeEntry);
};


#endif //RISOTTOV2_VARIABLESTABLE_H
