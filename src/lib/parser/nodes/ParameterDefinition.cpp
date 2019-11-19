//
// Created by rvigee on 10/2/19.
//

#include "ParameterDefinition.h"

#include <utility>

ParameterDefinition::ParameterDefinition(Token *name, TypeDescriptor *type, bool asReference)
        : name(name), type(type), asReference(asReference) {

}

ParameterDefinition::ParameterDefinition(const std::string &name, TypeDescriptor *type, bool asReference) :
        ParameterDefinition(Token::IdentifierFactory(name), type, asReference) {
}
