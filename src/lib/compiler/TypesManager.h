//
// Created by rvigee on 10/21/19.
//

#ifndef RISOTTOV2_TYPESMANAGER_H
#define RISOTTOV2_TYPESMANAGER_H


#include "TypeDefinition.h"

class TypesManager {
public:
    unsigned int functionCounter = 0;

    std::vector<InterfaceTypeDefinition *> interfaces;
    std::vector<TypeDefinition *> types;

    std::map<TypeDefinition *, std::vector<InterfaceTypeDefinition *>> implements;
    std::map<InterfaceTypeDefinition *, std::vector<TypeDefinition *>> implementedBy;

    void registerType(TypeDefinition *t) {
        if (auto interfaceType = dynamic_cast<InterfaceTypeDefinition *>(t)) {
            interfaces.push_back(interfaceType);

            for (auto type : types) {
                computeImplementations(type);
            }
        }

        types.push_back(t);

        computeImplementations(t);
    }

    void registerFunction(TypeDefinition *receiver, FunctionEntry *function) {
        function->addr = ++functionCounter;
        computeImplementations(receiver);

        auto entry = vtable_entry{.vaddr  = receiver->vtable->size++, .addr= i2v(function->firstByte->byte)};
        vec_push(&receiver->vtable->addrs, entry);
    }

    void computeImplementations(TypeDefinition *addedType) {
        for (auto interface: interfaces) {
            if (interface == addedType) {
                continue;
            }

            if (interface->getTypeReference()->canReceiveType(addedType->getTypeReference())) {
                if (implements.find(addedType) == implements.end()) {
                    implements[addedType] = std::vector<InterfaceTypeDefinition *>();
                }
                implements[addedType].push_back(interface);

                if (implementedBy.find(interface) == implementedBy.end()) {
                    implementedBy[interface] = std::vector<TypeDefinition *>();
                }
                implementedBy[interface].push_back(addedType);
            }
        }
    }
};


#endif //RISOTTOV2_TYPESMANAGER_H
