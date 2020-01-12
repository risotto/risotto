//
// Created by rvigee on 11/22/19.
//

#include "TypesManager.h"
#include "lib/parser/nodes/TypeDescriptor.h"
#include <sstream>

LinkUnit::LinkUnit(TypeDescriptor *typeDesc, Frame *frame, bool allowFindType) : typeDesc(typeDesc), frame(frame),
                                                                                 allowFindType(allowFindType) {

}

void TypesManager::add(TypeDescriptor *typeDesc, Frame *frame, bool allowFindType) {
    units.push_back(new LinkUnit(typeDesc, frame, allowFindType));

    typeDesc->createLinkUnits(this, frame);
}

void TypesManager::add(TypeDescriptor *typeDesc, Frame *frame) {
    add(typeDesc, frame, true);
}

void TypesManager::link() {
    bool hasAdvanced;
    do {
        hasAdvanced = false;
        auto uit = units.begin();

        while (uit != units.end()) {
            auto unit = *uit;
            auto typeDesc = unit->typeDesc;

            auto ok = typeDesc->resolveType(this, unit->frame, unit->allowFindType);
            if (ok) {
                hasAdvanced = true;
                uit = units.erase(uit);
                delete unit;

                registerType(typeDesc->getTypeDefinition());
            } else {
                unit->hasError = true;
                std::stringstream ss;
                ss << "Cannot find type for: " << typeDesc->toString();

                if (auto idDesc = dynamic_cast<IdentifierTypeDescriptor *>(typeDesc)) {
                    ss << " at " << idDesc->name->position.toString();
                }

                unit->lastError = ss.str();
                ++uit;
            }
        }

        if (hasAdvanced) {
            auto lit = listeners.begin();
            while (lit != listeners.end()) {
                auto listener = *lit;

                auto done = listener();

                if (done) {
                    lit = listeners.erase(lit);
                } else {
                    ++lit;
                }
            }
        }
    } while (hasAdvanced && !units.empty());

    if (!units.empty()) {
        std::stringstream ss;

        for (auto linkable: units) {
            if (linkable->hasError) {
                ss << linkable->lastError << std::endl;
            }
        }

        throw std::logic_error("cannot link: \n" + ss.str());
    }

    if (!listeners.empty()) {
        throw std::logic_error("all listeners didnt finish");
    }

    computeImplementations();
}

void TypesManager::addListener(const std::function<bool()> &listener) {
    listeners.push_back(listener);
}

void TypesManager::registerType(TypeDefinition *t) {
    if (auto interfaceType = dynamic_cast<InterfaceTypeDefinition *>(t)) {
        interfaces.insert(interfaceType);
    } else {
        types.insert(t);
    }
}

void TypesManager::registerFunction(TypeDefinition *receiver, FunctionEntry *function) {
    if (auto declEntry = dynamic_cast<DeclarationFunctionEntry *>(function)) {
        declEntry->addr = interfaceFunction++;
    } else {
        functions.insert(std::make_pair(receiver, function));
    }
}

void TypesManager::computeImplementations() {
    for (auto type: types) {
        for (auto interface: interfaces) {
            if (interface->canReceiveType(type)) {
                if (implements.find(type) == implements.end()) {
                    implements[type] = std::vector<InterfaceTypeDefinition *>();
                }
                implements[type].push_back(interface);

                if (implementedBy.find(interface) == implementedBy.end()) {
                    implementedBy[interface] = std::vector<TypeDefinition *>();
                }
                implementedBy[interface].push_back(type);

                for (auto function: type->functions.entries) {
                    for (auto iFunction: interface->functions.entries) {
                        if (auto declEntry = dynamic_cast<DeclarationFunctionEntry *>(iFunction)) {
                            if (function->isSame(iFunction)) {
                                function->vaddrs.push_back(declEntry->addr);
                            }
                        }
                    }
                }
            }
        }
    }
}

void TypesManager::generateVEntries(Compiler *compiler) {
    for (auto entry: functions) {
        generateVEntry(compiler, entry.first, entry.second);
    }
}

void TypesManager::generateVEntry(Compiler *compiler, TypeDefinition *receiver, FunctionEntry *function) {
    Value addr;
    if (auto native = dynamic_cast<NativeFunctionEntry *>(function)) {
        addr = p2v((void *) native->fun);
    } else if (auto code = dynamic_cast<CodeFunctionEntry *>(function)) {
        addr = i2v(compiler->getAddr(code->firstByte));
    } else if (dynamic_cast<BytesFunctionEntry *>(function)) {
        return; // Not supported
    } else {
        throw std::logic_error("Unhandled function entry");
    }

    for (auto vaddr: function->vaddrs) {
        auto entry = vtable_entry{
                .vaddr = vaddr,
                .addr = addr,
        };
        vec_push(&receiver->vtable->addrs, entry);
    }
}
