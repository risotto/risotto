//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_PARAMETERDEFINITION_H
#define RISOTTOV2_PARAMETERDEFINITION_H


#include <lib/tokenizer/Token.h>
#include "TypeDescriptor.h"

class ParameterDefinition {
public:
    Token *name;
    TypeDescriptor *type;
    bool asReference;

    ParameterDefinition(Token *name, TypeDescriptor *type, bool asReference);
};


#endif //RISOTTOV2_PARAMETERDEFINITION_H
