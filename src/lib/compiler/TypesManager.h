//
// Created by rvigee on 11/22/19.
//

#ifndef RISOTTOV2_TYPESMANAGER_H
#define RISOTTOV2_TYPESMANAGER_H


#include "Frame.h"

class LinkUnit {
public:
    TypeDescriptor *typeDesc;
    Frame *frame;
    bool hasError = false;
    std::string lastError;

    LinkUnit(TypeDescriptor *typeDesc, Frame *frame);
};

class TypesManager {
private:
    std::vector<LinkUnit *> units;
public:
    void add(TypeDescriptor *typeDesc, Frame *frame);

    void link();
};


#endif //RISOTTOV2_TYPESMANAGER_H
