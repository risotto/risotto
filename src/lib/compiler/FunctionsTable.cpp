//
// Created by rvigee on 10/4/19.
//

#include "FunctionsTable.h"
#include "lib/compiler/utils/Utils.h"
#include "lib/compiler/ReturnTypes.h"
#include "TypeDefinition.h"
#include "lib/parser/nodes/TypeDescriptor.h"

#include <utility>

FunctionEntry::FunctionEntry(std::string name, FunctionTypeDescriptor *descriptor) :
        name(std::move(name)), descriptor(descriptor) {
}

bool FunctionEntry::isSame(FunctionEntry *other) {
    if (this == other) {
        return true;
    }

    if (name != other->name) {
        return false;
    }

    return descriptor->isSame(other->descriptor);
}

NativeFunctionEntry::NativeFunctionEntry(
        std::string name,
        FunctionTypeDescriptor *descriptor,
        NativeFunction fun
) : FunctionEntry(std::move(name), descriptor), fun(fun) {

}

FunctionEntry *FunctionsTable::find(const std::string &name, const std::vector<TypeDescriptor *> &argsTypes) {
    auto functionsWithName = findCandidates(name);

    return Utils::findMatchingFunctions(functionsWithName, argsTypes);
}

FunctionEntry *FunctionsTable::add(FunctionEntry *entry) {
    auto paramsTypes = std::vector<TypeDescriptor *>();
    for (const auto &param: entry->descriptor->params) {
        paramsTypes.push_back(param->type);
    }

    auto existingFunction = find(entry->name, paramsTypes);

    if (existingFunction != nullptr) {
        auto it = std::find(entries.begin(), entries.end(), existingFunction);

        if (it != entries.end()) {
            auto index = std::distance(entries.begin(), it);

            entries.erase(entries.begin() + index);
        }
    }

    entries.push_back(entry);

    return entry;
}

std::vector<FunctionEntry *> FunctionsTable::findCandidates(const std::string &name) {
    return Utils::findCandidatesFunctions(entries, name);
}

BytesFunctionEntry::BytesFunctionEntry(
        const std::string &name,
        FunctionTypeDescriptor *descriptor,
        std::function<std::vector<ByteResolver *>()> generator
) : FunctionEntry(name, descriptor), generator(std::move(generator)) {}

std::vector<ByteResolver *> BytesFunctionEntry::get() {
    return generator();
}
