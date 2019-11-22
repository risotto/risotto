//
// Created by rvigee on 10/8/19.
//

#ifndef RISOTTOV2_TYPEDESCRIPTOR_H
#define RISOTTOV2_TYPEDESCRIPTOR_H


#include <lib/compiler/Compiler.h>
#include <functional>

class Compiler;

class Frame;

class TypeDescriptor {
public:
    virtual std::string toString() = 0;

    virtual TypeDefinition *genType(Frame *frame) = 0;

    bool resolveType(Frame *frame);

    virtual void createLinkUnits(TypesManager *typesManager, Frame *frame) = 0;

    TypeDefinition *getTypeDefinition();

    bool canReceiveType(TypeDescriptor *type);

    virtual bool isSame(TypeDescriptor *type);

protected:
    TypeDefinition *typeDef = nullptr;
};

class ArrayTypeDescriptor : public TypeDescriptor {
public:
    TypeDescriptor *element;

    explicit ArrayTypeDescriptor(TypeDescriptor *element);

    TypeDefinition *genType(Frame *frame) override;

    std::string toString() override;

    bool isSame(TypeDescriptor *type) override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
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

    TypeDefinition *genType(Frame *frame) override;

    std::string toString() override;

    bool isSame(TypeDescriptor *type) override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};

class IdentifierTypeDescriptor : public TypeDescriptor {
public:
    Token *name;
    std::function<TypeDefinition *(Frame *frame)> typeDefGen = nullptr;

    explicit IdentifierTypeDescriptor(Token *name);

    IdentifierTypeDescriptor(const std::string &name, TypeDefinition *typeDef);

    IdentifierTypeDescriptor(Token *name, std::function<TypeDefinition *(Frame *frame)> typeDefGen);

    TypeDefinition *genType(Frame *frame) override;

    bool isSame(TypeDescriptor *type) override;

    std::string toString() override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};

class FunctionTypeDescriptor : public TypeDescriptor {
public:
    std::vector<ParameterDefinition *> params;
    std::vector<TypeDescriptor *> returnTypes;

    FunctionTypeDescriptor(std::vector<ParameterDefinition *> params, std::vector<TypeDescriptor *> returnTypes);

    explicit FunctionTypeDescriptor(FunctionEntry *functionEntry);

    TypeDefinition *genType(Frame *frame) override;

    std::string toString() override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};


#endif //RISOTTOV2_TYPEDESCRIPTOR_H
