//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_TYPESTABLE_H
#define RISOTTOV2_TYPESTABLE_H

#include <string>
#include <vector>
#include <map>

class TypeDescriptor;

class TypesTable {
public:
    std::vector<TypeDescriptor *> entries;

    TypeDescriptor *findNamed(const std::string &name);

    TypeDescriptor *find(TypeDescriptor *desc);

    TypeDescriptor *add(TypeDescriptor *typeEntry);

    TypeDescriptor *add(TypeDescriptor *typeEntry, bool allowFindType);
};

#endif //RISOTTOV2_TYPESTABLE_H
