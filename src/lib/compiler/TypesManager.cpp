//
// Created by rvigee on 11/22/19.
//

#include "TypesManager.h"
#include "lib/parser/nodes/TypeDescriptor.h"
#include <sstream>

LinkUnit::LinkUnit(TypeDescriptor *typeDesc, Frame *frame) : typeDesc(typeDesc), frame(frame) {

}

void TypesManager::add(TypeDescriptor *typeDesc, Frame *frame) {
    units.push_back(new LinkUnit(typeDesc, frame));

    typeDesc->createLinkUnits(this, frame);
}

void TypesManager::link() {
    bool hasAdvanced;
    do {
        hasAdvanced = false;
        auto it = units.begin();

        while (it != units.end()) {
            auto unit = *it;
            auto typeDesc = unit->typeDesc;

            auto ok = typeDesc->resolveType(unit->frame);
            if (ok) {
                hasAdvanced = true;
                it = units.erase(it);
                delete unit;
            } else {
                unit->hasError = true;
                std::stringstream ss;
                ss << "Cannot find type for: " << typeDesc->toString();

                if (auto idDesc = dynamic_cast<IdentifierTypeDescriptor *>(typeDesc)) {
                    ss << " at " << idDesc->name->position.toString();
                }

                unit->lastError = ss.str();
                ++it;
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
}
