//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_PARAMETERDEFINITION_H
#define RISOTTOV2_PARAMETERDEFINITION_H


#include <lib/tokenizer/Token.h>

class TypeDescriptor;

class ParameterDefinition {
public:
    Token *name;
    TypeDescriptor *type;
    bool asReference;

    ParameterDefinition(Token *name, TypeDescriptor *type, bool asReference);
};


#endif //RISOTTOV2_PARAMETERDEFINITION_H
