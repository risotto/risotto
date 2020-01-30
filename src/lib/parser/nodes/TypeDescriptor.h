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

    virtual TypeDefinition *genType(TypesManager *typesManager, Frame *frame) = 0;

    bool resolveType(TypesManager *typesManager, Frame *frame, bool allowFindType);

    virtual void createLinkUnits(TypesManager *typesManager, Frame *frame) = 0;

    TypeDefinition *getTypeDefinition();

    bool canReceiveType(TypeDescriptor *type);

    virtual bool isSame(TypeDescriptor *type);

    TypeDefinition *typeDef = nullptr;
};

class NilTypeDescriptor: public TypeDescriptor {
public:
    Token *token;

    static NilTypeDescriptor Def;

    NilTypeDescriptor();
    explicit NilTypeDescriptor(Token *name);

    std::string toString() override;

    TypeDefinition *genType(TypesManager *typesManager, Frame *frame) override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;

    bool isSame(TypeDescriptor *type) override;
};

class ArrayTypeDescriptor : public TypeDescriptor {
public:
    TypeDescriptor *element;

    explicit ArrayTypeDescriptor(TypeDescriptor *element);

    TypeDefinition *genType(TypesManager *typesManager, Frame *frame) override;

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

    TypeDefinition *genType(TypesManager *typesManager, Frame *frame) override;

    std::string toString() override;

    bool isSame(TypeDescriptor *type) override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};

class IdentifierTypeDescriptor : public TypeDescriptor {
public:
    Token *name;
    TypeDescriptor *typeDesc = nullptr;

    explicit IdentifierTypeDescriptor(Token *name);

    IdentifierTypeDescriptor(Token *name, TypeDescriptor *typeDesc);

    IdentifierTypeDescriptor(const std::string &name, TypeDefinition *typeDef);

    TypeDefinition *genType(TypesManager *typesManager, Frame *frame) override;

    bool isSame(TypeDescriptor *type) override;

    std::string toString() override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};

class FunctionTypeDescriptor : public TypeDescriptor {
public:
    bool isMethod;
    std::vector<ParameterDefinition *> params;
    std::vector<TypeDescriptor *> returnTypes;

    FunctionTypeDescriptor(bool isMethod, std::vector<ParameterDefinition *> params,
                           std::vector<TypeDescriptor *> returnTypes);

    TypeDefinition *genType(TypesManager *typesManager, Frame *frame) override;

    std::string toString() override;

    bool isSame(TypeDescriptor *type) override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};

class InterfaceTypeDescriptor : public TypeDescriptor {
public:
    std::vector<FunctionEntry *> functions;

    explicit InterfaceTypeDescriptor(std::vector<FunctionEntry *> functions);

    TypeDefinition *genType(TypesManager *typesManager, Frame *frame) override;

    std::string toString() override;

    bool isSame(TypeDescriptor *type) override;

    void createLinkUnits(TypesManager *typesManager, Frame *frame) override;
};


#endif //RISOTTOV2_TYPEDESCRIPTOR_H
