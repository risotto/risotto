//
// Created by rvigee on 10/2/19.
//

#include "ParameterDefinition.h"

ParameterDefinition::ParameterDefinition(Token *name, TypeDescriptor type, bool asReference)
        : name(name), type(type), asReference(asReference) {

}
