//
// Created by rvigee on 10/11/19.
//

#include "lib/compiler/ReturnTypes.h"

ReturnTypes::ReturnTypes(TypeReference *ref) : ReturnTypes({ref}) {
}

ReturnTypes::ReturnTypes(std::vector<TypeReference *> refs): ReturnTypes() {
    this->insert(this->end(),  refs.begin(), refs.end());
}

bool ReturnTypes::single() {
    return size() == 1;
}
