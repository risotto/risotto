//
// Created by rvigee on 10/8/19.
//

#ifndef RISOTTOV2_TYPEDESCRIPTOR_H
#define RISOTTOV2_TYPEDESCRIPTOR_H


#include <lib/tokenizer/Token.h>
#include <lib/compiler/TypeReference.h>

class Compiler;

class TypeDescriptor {
public:
    Token *name;
    bool isArray;

    TypeDescriptor(Token *name, bool isArray);

    TypeReference toTypeReference(Compiler *compiler);
};


#endif //RISOTTOV2_TYPEDESCRIPTOR_H
