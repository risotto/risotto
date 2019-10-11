//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_TYPESTABLE_H
#define RISOTTOV2_TYPESTABLE_H

#include <string>
#include <vector>
#include "TypeEntry.h"

class TypesTable {
private:
    std::vector<TypeEntry *> entries;
public:
    TypeEntry *findNamed(const std::string &name);

    TypeEntry *add(std::string name);
};


#endif //RISOTTOV2_TYPESTABLE_H
