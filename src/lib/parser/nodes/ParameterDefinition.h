//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_PARAMETERDEFINITION_H
#define RISOTTOV2_PARAMETERDEFINITION_H


#include <lib/tokenizer/Token.h>

class ParameterDefinition {
public:
    Token *name;
    Token *type;
    bool isReference;

    ParameterDefinition(Token *name, Token *type, bool isReference);
};


#endif //RISOTTOV2_PARAMETERDEFINITION_H
