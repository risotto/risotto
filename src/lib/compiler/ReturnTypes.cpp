//
// Created by rvigee on 10/11/19.
//

#include "lib/compiler/ReturnTypes.h"

ReturnTypes::ReturnTypes(TypeReference *ref) : ReturnTypes({ref}) {
}

ReturnTypes::ReturnTypes(std::vector<TypeReference *> refs): ReturnTypes() {
    this->insert(this->end(),  refs.begin(), refs.end());
}

ReturnTypes ReturnTypes::onlyFunctions() {
    auto onlyFunctions = ReturnTypes();

    for (auto ref : *this) {
        if (ref->asFunction()) {
            onlyFunctions.push_back(ref);
        }
    }

    return onlyFunctions;
}

bool ReturnTypes::single() {
    return size() == 1;
}
