//
// Created by rvigee on 10/3/19.
//

#include <lib/parser/nodes/TypeDescriptor.h>
#include "Frame.h"

VariableFindResponse::VariableFindResponse(VariableEntry *variable, int distance) : variable(variable),
                                                                                    distance(distance) {}

Frame::Frame(FrameType type) : type(type) {

}

Frame::Frame(Frame *parent) : parent(parent) {

}

Frame::Frame(Frame *parent, FrameType type) : parent(parent), type(type) {

}

TypeEntry *Frame::findNamedType(const std::string& name) {
    auto entry = types.findNamed(name);

    if (entry != nullptr) {
        return entry;
    }

    if (parent != nullptr) {
        return parent->findNamedType(name);
    }

    return nullptr;
}

VariableFindResponse *Frame::findVariable(const std::string &name) {
    Frame *current = this;
    int distance = 0;
    while (current != nullptr) {
        auto entry = current->variables.find(name);

        if (entry != nullptr) {
            return new VariableFindResponse(entry, distance);
        }

        distance++;
        current = current->parent;
    }

    return nullptr;
}

int Frame::findFrame(FrameType type) {
    Frame *current = this;
    int distance = 0;
    while (current != nullptr) {
        if (current->type == type) {
            return distance;
        }

        distance++;
        current = current->parent;
    }

    return -1;
}

std::vector<FunctionEntry *> Frame::findFunctionsCandidates(const std::string &name) {
    auto allCandidates = std::vector<FunctionEntry *>();

    Frame *current = this;
    while (current != nullptr) {
        auto functionCandidates = current->functions.findCandidates(name);
        allCandidates.insert(allCandidates.end(), functionCandidates.begin(), functionCandidates.end());

        auto var = current->variables.find(name);
        if (var != nullptr) {
            if (var->typeRef->isFunction()) {
                if (auto functionTypeRef = dynamic_cast<FunctionTypeReference *>(var->typeRef)) {
                    allCandidates.push_back(functionTypeRef->entry->function);
                }
            }
        }

        current = current->parent;
    }

    return allCandidates;
}
