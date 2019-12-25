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
    TypeDescriptor *typeRef;
    int index;

    VariableEntry(std::string name, TypeDescriptor *typeRef, int index);
};

class VariablesTable {
private:
    std::vector<VariableEntry *> entries;

    typedef std::vector<VariableEntry *>::const_iterator iterator;
public:
    VariableEntry *find(const std::string &name);

    VariableEntry *add(std::string name, TypeDescriptor *typeRef);

    [[nodiscard]] iterator begin() const { return entries.begin(); }

    [[nodiscard]] iterator end() const { return entries.end(); }

    unsigned long size() {
        return entries.size();
    }
};


#endif //RISOTTOV2_VARIABLESTABLE_H
