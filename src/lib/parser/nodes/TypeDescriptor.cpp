//
// Created by rvigee on 10/8/19.
//

extern "C" {
#include "lib/vm/native_functions.h"
}

#include "TypeDescriptor.h"
#include <lib/compiler/CompilerError.h>
#include <utility>
#include <sstream>
#include <cassert>
#include <lib/compiler/utils/Utils.h>
#include "lib/compiler/Compiler.h"
#include "lib/compiler/TypeDefinition.h"

NilTypeDescriptor NilTypeDescriptor::Def = NilTypeDescriptor();

NilTypeDescriptor::NilTypeDescriptor(): NilTypeDescriptor(Token::IdentifierFactory("nil"))  {}

NilTypeDescriptor::NilTypeDescriptor(PToken token): token(token) {
    typeDef = &NilTypeDefinition::Def;
}

std::string NilTypeDescriptor::toString() {
    return token->lexeme;
}

TypeDefinition *NilTypeDescriptor::genType(TypesManager *typesManager, Frame *frame) {
    return typeDef;
}

void NilTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {

}

bool NilTypeDescriptor::isSame(TypeDescriptor *type) {
    return true;
}

std::string IdentifierTypeDescriptor::toString() {
    return name->lexeme;
}

IdentifierTypeDescriptor::IdentifierTypeDescriptor(PToken name) : name(name) {
}

IdentifierTypeDescriptor::IdentifierTypeDescriptor(const std::string &name, TypeDefinition *typeDef) :
        IdentifierTypeDescriptor(Token::IdentifierFactory(name)) {
    this->typeDef = typeDef;
}

IdentifierTypeDescriptor::IdentifierTypeDescriptor(PToken name, TypeDescriptor *typeDesc) : IdentifierTypeDescriptor(
        name) {
    this->typeDesc = typeDesc;
}

TypeDefinition *IdentifierTypeDescriptor::genType(TypesManager *typesManager, Frame *frame) {
    TypeDescriptor *desc;
    if (typeDesc != nullptr) {
        desc = typeDesc;
    } else {
        desc = frame->findNamedType(name->lexeme);
    }

    if (desc == nullptr) {
        return nullptr;
    }

    return desc->getTypeDefinition();
}

bool IdentifierTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto otherIdentifier = dynamic_cast<IdentifierTypeDescriptor *>(type)) {
        return name->lexeme == otherIdentifier->name->lexeme;
    }

    return false;
}

void IdentifierTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    if (typeDesc) {
        typesManager->createLinkUnits(typeDesc, frame);
    }
}

ArrayTypeDescriptor::ArrayTypeDescriptor(TypeDescriptor *element) : element(element) {

}

std::string ArrayTypeDescriptor::toString() {
    return "[]" + element->toString();
}

TypeDefinition *ArrayTypeDescriptor::genType(TypesManager *typesManager, Frame *frame) {
    auto def = new ArrayTypeDefinition(element);

    // TODO: Remove when generic in place

    auto intDesc = frame->findNamedType("int");

    def->addOperator(
            new ParameterDefinition("a", this, true),
            new NativeFunctionEntry(
                    "[]",
                    new FunctionTypeDescriptor(
                            true,
                            {new ParameterDefinition("i", intDesc, false)},
                            {element}
                    ),
                    array_at
            )
    );

    def->addFunction(
            new ParameterDefinition("a", this, true),
            new NativeFunctionEntry(
                    "push",
                    new FunctionTypeDescriptor(
                            true,
                            {new ParameterDefinition("e", element, false)},
                            {}
                    ),
                    array_add
            )
    );

    def->addFunction(
            new ParameterDefinition("a", this, true),
            new NativeFunctionEntry(
                    "size",
                    new FunctionTypeDescriptor(
                            true,
                            {},
                            {intDesc}
                    ),
                    array_size
            )
    );

    return def;
}

bool ArrayTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto otherArray = dynamic_cast<ArrayTypeDescriptor *>(type)) {
        return element->isSame(otherArray->element);
    }

    return false;
}

void ArrayTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    typesManager->createLinkUnits(element, frame);
}

StructTypeDescriptor::StructTypeDescriptor(std::vector<Field> fields) : fields(std::move(fields)) {}

std::string StructTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "struct { ";

    for (auto field:fields) {
        ss << field.name->lexeme << " " << field.type->toString() << "; ";
    }

    ss << "}";

    return ss.str();
}

TypeDefinition *StructTypeDescriptor::genType(TypesManager *typesManager, Frame *frame) {
    auto structFields = VariablesTable();
    for (auto field: fields) {
        structFields.add(field.name->lexeme, field.type);
    }

    return new StructTypeDefinition(structFields);
}

void StructTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    for (auto field: fields) {
        typesManager->createLinkUnits(field.type, frame);
    }
}

bool StructTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto structDef = dynamic_cast<StructTypeDescriptor *>(type)) {
        if (structDef->fields.size() == fields.size()) {
            for (auto i = 0u; i < fields.size(); ++i) {
                auto field = fields[i];
                auto otherField = structDef->fields[i];

                if (!field.type->isSame(otherField.type)) {
                    return false;
                }
            }
        }
    }

    return false;
}

StructTypeDescriptor::Field::Field(PToken name, TypeDescriptor *type) : name(name), type(type) {}

FunctionTypeDescriptor::FunctionTypeDescriptor(
        bool isMethod,
        std::vector<ParameterDefinition *> params,
        std::vector<TypeDescriptor *> returnTypes
) : isMethod(isMethod), params(std::move(params)), returnTypes(std::move(returnTypes)) {}

std::string FunctionTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "func (";

    for (auto i = 0u; i < params.size(); ++i) {
        auto param = params[i];

        if (i != 0) {
            ss << ", ";
        }

        ss << param->name->lexeme << " " << param->type->toString();
    }

    ss << ")";

    if (!returnTypes.empty()) {
        ss << " (";
        for (auto i = 0u; i < returnTypes.size(); ++i) {
            auto type = returnTypes[i];

            if (i != 0) {
                ss << ", ";
            }

            ss << type->toString();
        }
        ss << ")";
    }

    return ss.str();
}

TypeDefinition *FunctionTypeDescriptor::genType(TypesManager *typesManager, Frame *frame) {
    return new FunctionTypeDefinition(this);
}

void FunctionTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    for (auto param: params) {
        typesManager->createLinkUnits(param->type, frame);
    }

    for (auto returnType: returnTypes) {
        typesManager->createLinkUnits(returnType, frame);
    }
}

bool FunctionTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto functionType = dynamic_cast<FunctionTypeDescriptor *>(type)) {
        auto shouldSkipFirst = this->isMethod;
        if (!Utils::typesMatch(params, functionType->params,
                               [shouldSkipFirst](int i, TypeDescriptor *l, TypeDescriptor *r) {
                                   if (i == 0 && shouldSkipFirst) {
                                       return true;
                                   }

                                   return Utils::TypesSame(i, l, r);
                               })) {
            return false;
        }

        return Utils::typesMatch(returnTypes, functionType->returnTypes, Utils::TypesSame);
    }

    return false;
}

TypeDefinition *TypeDescriptor::getTypeDefinition() {
    return typeDef;
}

bool TypeDescriptor::canReceiveType(TypeDescriptor *type) {
    auto typeDef = getTypeDefinition();
    auto otherTypeDef = type->getTypeDefinition();

    assert(typeDef != nullptr);
    assert(otherTypeDef != nullptr);

    return typeDef->canReceiveType(otherTypeDef);
}

bool TypeDescriptor::isSame(TypeDescriptor *type) {
    return this == type;
}

bool TypeDescriptor::resolveType(TypesManager *typesManager, Frame *frame, bool allowFindType) {
    if (typeDef != nullptr) {
        return true;
    }

    if (allowFindType) {
        if (auto desc = frame->findType(this)) {
            typeDef = desc->getTypeDefinition();
        } else {
            typeDef = genType(typesManager, frame);
        }
    } else {
        typeDef = genType(typesManager, frame);
    }

    return typeDef != nullptr;
}

InterfaceTypeDescriptor::InterfaceTypeDescriptor(std::vector<FunctionEntry *> functions) :
        functions(std::move(functions)) {
}

TypeDefinition *InterfaceTypeDescriptor::genType(TypesManager *typesManager, Frame *frame) {
    auto def = new InterfaceTypeDefinition(this, functions);

    for (auto function: functions) {
        typesManager->registerFunction(def, function);
    }

    return def;
}

std::string InterfaceTypeDescriptor::toString() {
    std::stringstream ss;

    ss << "interface {";

    if (!functions.empty()) {
        ss << "\n";

        for (auto func: functions) {
            ss << func->name << ":" << func->descriptor->toString() << "\n";
        }
    }

    ss << "}";

    return ss.str();
}

bool InterfaceTypeDescriptor::isSame(TypeDescriptor *type) {
    if (TypeDescriptor::isSame(type)) {
        return true;
    }

    if (auto otherInterface = dynamic_cast<InterfaceTypeDescriptor *>(type)) {
        if (functions.size() != otherInterface->functions.size()) {
            return false;
        }

        for (auto func : functions) {
            auto has = false;

            for (auto otherFunc: otherInterface->functions) {
                if (func->isSame(otherFunc)) {
                    has = true;
                    break;
                }
            }

            if (!has) {
                return false;
            }
        }
    }

    return false;
}

void InterfaceTypeDescriptor::createLinkUnits(TypesManager *typesManager, Frame *frame) {
    for (const auto &func: functions) {
        for (auto param: func->descriptor->params) {
            typesManager->createLinkUnits(param->type, frame);
        }

        for (auto returnType: func->descriptor->returnTypes) {
            typesManager->createLinkUnits(returnType, frame);
        }
    }
}
