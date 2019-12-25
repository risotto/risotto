//
// Created by rvigee on 11/22/19.
//

#include "TypesManager.h"
#include "lib/parser/nodes/TypeDescriptor.h"
#include <sstream>

LinkUnit::LinkUnit(TypeDescriptor *typeDesc, Frame *frame, bool allowFindType) : typeDesc(typeDesc), frame(frame), allowFindType(allowFindType) {

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

            auto ok = typeDesc->resolveType(unit->frame, unit->allowFindType);
            if (ok) {
                hasAdvanced = true;
                uit = units.erase(uit);
                delete unit;
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
}

void TypesManager::addListener(const std::function<bool()>& listener) {
    listeners.push_back(listener);
}
