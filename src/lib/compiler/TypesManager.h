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
    bool allowFindType;

    bool hasError = false;
    std::string lastError;

    LinkUnit(TypeDescriptor *typeDesc, Frame *frame, bool allowFindType);
};

class TypesManager {
private:
    std::vector<LinkUnit *> units;
    std::vector<std::function<bool()>> listeners;
public:
    void add(TypeDescriptor *typeDesc, Frame *frame, bool allowFindType);
    void add(TypeDescriptor *typeDesc, Frame *frame);

    void addListener(const std::function<bool()>& f);

    void link();
};


#endif //RISOTTOV2_TYPESMANAGER_H
