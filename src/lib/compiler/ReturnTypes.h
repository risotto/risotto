//
// Created by rvigee on 10/11/19.
//

#ifndef RISOTTOV2_RETURNTYPES_H
#define RISOTTOV2_RETURNTYPES_H


#include "TypeReference.h"

class ReturnTypes : public std::vector<TypeReference *> {
public:
    using std::vector<TypeReference *>::vector;

    ReturnTypes(TypeReference *ref);
    ReturnTypes(std::vector<TypeReference *> refs);

    bool single();
};

#endif //RISOTTOV2_RETURNTYPES_H
