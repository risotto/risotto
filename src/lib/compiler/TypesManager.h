//
// Created by rvigee on 11/22/19.
//

#ifndef RISOTTOV2_TYPESMANAGER_H
#define RISOTTOV2_TYPESMANAGER_H


#include <set>
#include "Frame.h"
#include "TypeDefinition.h"

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

    int interfaceFunction = 0;

    std::set<InterfaceTypeDefinition *> interfaces;
    std::set<TypeDefinition *> types;
    std::set<std::pair<TypeDefinition *, FunctionEntry *>> functions;

    std::map<TypeDefinition *, std::vector<InterfaceTypeDefinition *>> implements;
    std::map<InterfaceTypeDefinition *, std::vector<TypeDefinition *>> implementedBy;

public:
    void createLinkUnits(TypeDescriptor *typeDesc, Frame *frame, bool allowFindType);
    void createLinkUnits(TypeDescriptor *typeDesc, Frame *frame);

    void runListeners();
    void addListener(const std::function<bool()>& f);

    void link();

    void registerType(TypeDefinition *t);
    void registerFunction(TypeDefinition *receiver, FunctionEntry *function);
    void generateVEntry(Compiler *compiler, TypeDefinition *receiver, FunctionEntry *function);
    void generateVEntries(Compiler *compiler);
    void computeImplementations();
};


#endif //RISOTTOV2_TYPESMANAGER_H
