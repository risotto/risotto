//
// Created by rvigee on 10/2/19.
//

#include "ParameterDefinition.h"

ParameterDefinition::ParameterDefinition(Token *name, Token *type, bool isReference)
        : name(name), type(type), isReference(isReference) {

}
