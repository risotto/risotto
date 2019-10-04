//
// Created by rvigee on 10/4/19.
//

#ifndef RISOTTOV2_TYPEREFERENCE_H
#define RISOTTOV2_TYPEREFERENCE_H


#include "TypesTable.h"

class TypeReference {
public:
    TypeEntry *typeEntry = nullptr;
    std::string typeName = "";

    TypeEntry *getType(Compiler *c);
};


#endif //RISOTTOV2_TYPEREFERENCE_H
