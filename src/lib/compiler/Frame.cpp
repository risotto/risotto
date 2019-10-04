//
// Created by rvigee on 10/3/19.
//

#include "Frame.h"

VariableFindResponse::VariableFindResponse(VariableEntry *variable, int distance) : variable(variable),
                                                                                    distance(distance) {}

Frame::Frame(FrameType type) : type(type) {

}

Frame::Frame(Frame *parent) : parent(parent) {

}

Frame::Frame(Frame *parent, FrameType type) : parent(parent), type(type) {

}

TypeEntry *Frame::findType(const std::string &name) {
    auto entry = types.find(name);

    if (entry != nullptr) {
        return entry;
    }

    if (parent != nullptr) {
        return parent->findType(name);
    }

    return nullptr;
}

VariableFindResponse *Frame::findVariable(const std::string &name) {
    Frame *current = this;
    int distance = 0;
    while (current != nullptr) {
        auto entry = variables.find(name);

        if (entry != nullptr) {
            return new VariableFindResponse(entry, distance);
        }

        distance++;
        current = current->parent;
    }

    return nullptr;
}

FunctionEntry *Frame::findFunction(const std::string &name) {
    auto entry = functions.find(name);

    if (entry != nullptr) {
        return entry;
    }

    if (parent != nullptr) {
        return parent->findFunction(name);
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
