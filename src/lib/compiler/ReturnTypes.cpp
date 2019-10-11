//
// Created by rvigee on 10/11/19.
//

#include "lib/compiler/ReturnTypes.h"

ReturnTypes::ReturnTypes(TypeReference *ref) : ReturnTypes({ref}) {
}

ReturnTypes ReturnTypes::onlyFunctions() {
    auto onlyFunctions = ReturnTypes();

    for (auto ref : *this) {
        if (ref->isFunction()) {
            onlyFunctions.push_back(ref);
        }
    }

    return onlyFunctions;
}

bool ReturnTypes::single() {
    return size() == 1;
}