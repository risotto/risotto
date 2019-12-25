//
// Created by rvigee on 10/11/19.
//

#ifndef RISOTTOV2_RETURNTYPES_H
#define RISOTTOV2_RETURNTYPES_H

#include <vector>

class TypeDescriptor;

class ReturnTypes : public std::vector<TypeDescriptor *> {
public:
    using std::vector<TypeDescriptor *>::vector;

    ReturnTypes(TypeDescriptor *ref);
    ReturnTypes(std::vector<TypeDescriptor *> refs);

    bool single();
};

#endif //RISOTTOV2_RETURNTYPES_H
