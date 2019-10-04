//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_TYPESTABLE_H
#define RISOTTOV2_TYPESTABLE_H

#include <string>
#include <vector>
#include "FunctionsTable.h"

class TypeEntry {
public:
    TypeEntry(std::string name);

    std::string name;
    FunctionsTable functions;
    FunctionsTable operators;

    bool canReceiveType(TypeEntry *type);

    FunctionEntry *addFunction(const std::string& selfName, FunctionEntry *entry);
    FunctionEntry *addOperator(const std::string& selfName, FunctionEntry *entry);
};

class TypesTable {
private:
    std::vector<TypeEntry *> types;
public:
    TypeEntry *find(const std::string& name);
    TypeEntry *add(std::string name);
};


#endif //RISOTTOV2_TYPESTABLE_H
