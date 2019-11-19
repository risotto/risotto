//
// Created by rvigee on 10/8/19.
//

#ifndef RISOTTOV2_TYPEDESCRIPTOR_H
#define RISOTTOV2_TYPEDESCRIPTOR_H


#include <lib/tokenizer/Token.h>
#include <lib/compiler/ReturnTypes.h>
#include <lib/compiler/TypesTable.h>
#include <lib/compiler/FunctionsTable.h>
#include "ParameterDefinition.h"

class Compiler;

class Frame;

class TypeDescriptor {
public:
    virtual std::string toString() = 0;

    virtual TypeEntry *genType(Frame *frame) = 0;

    TypeDefinition *getTypeDefinition();

    void setTypeEntry(TypeEntry *entry);

    bool canReceiveType(TypeDescriptor *type);
    virtual bool isSame(TypeDescriptor *type);

protected:
    TypeEntry *typeEntry = nullptr;
};

class ArrayTypeDescriptor : public TypeDescriptor {
public:
    TypeDescriptor *element;

    explicit ArrayTypeDescriptor(TypeDescriptor *element);

    TypeEntry *genType(Frame *frame) override;

    std::string toString() override;

    bool isSame(TypeDescriptor *type) override;
};

class StructTypeDescriptor : public TypeDescriptor {
public:
    class Field {
    public:
        Token *name;
        TypeDescriptor *type;

        Field(Token *name, TypeDescriptor *type);
    };

    std::vector<Field> fields;

    explicit StructTypeDescriptor(std::vector<Field> fields);

    TypeEntry *genType(Frame *frame) override;

    std::string toString() override;
};

class IdentifierTypeDescriptor : public TypeDescriptor {
public:
    Token *name;

    explicit IdentifierTypeDescriptor(Token *name);

    IdentifierTypeDescriptor(const std::string &name, TypeDefinition *typeDef);

    TypeEntry *genType(Frame *frame) override;

    bool isSame(TypeDescriptor *type) override;

    std::string toString() override;
};

class FunctionTypeDescriptor : public TypeDescriptor {
public:
    std::vector<ParameterDefinition> params;
    std::vector<TypeDescriptor *> returnTypes;

    FunctionTypeDescriptor(std::vector<ParameterDefinition> params, std::vector<TypeDescriptor *> returnTypes);
    explicit FunctionTypeDescriptor(FunctionEntry *functionEntry);

    TypeEntry *genType(Frame *frame) override;

    std::string toString() override;
};


#endif //RISOTTOV2_TYPEDESCRIPTOR_H
