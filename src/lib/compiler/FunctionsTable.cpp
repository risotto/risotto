//
// Created by rvigee on 10/4/19.
//

#include "FunctionsTable.h"
#include "TypesTable.h"
#include "lib/compiler/utils/Utils.h"

#include <utility>

bool TypesEntries::single() {
    return size() == 1;
}

TypesEntries::TypesEntries(TypeEntry *entry) : std::vector<TypeEntry *>({entry}) {
}

TypesEntries TypesEntries::onlyFunctions() {
    auto onlyFunctions = TypesEntries();

    for (auto entry : *this) {
        if (entry->isFunction()) {
            onlyFunctions.push_back(entry);
        }
    }

    return onlyFunctions;
}

FunctionEntryParameter::FunctionEntryParameter(std::string name, TypeEntry *type) : FunctionEntryParameter(std::move(name), type, false) {}
FunctionEntryParameter::FunctionEntryParameter(std::string name, TypeEntry *type, bool isReference) : name(std::move(name)), type(type), isReference(isReference) {}

FunctionEntry::FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypesEntries returnTypes) :
        name(std::move(name)), params(std::move(params)), returnTypes(std::move(returnTypes)) {
    typeEntry = new FunctionTypeEntry(name, this);
}

NativeFunctionEntry::NativeFunctionEntry(
        std::string name,
        std::vector<FunctionEntryParameter> params,
        TypesEntries returnTypes,
        NativeFunctionReturn (*fun)(Value[], int)
) : FunctionEntry(std::move(name), std::move(params), std::move(returnTypes)), fun(fun) {

}

FunctionEntry *FunctionsTable::find(const std::string &name, const std::vector<TypeEntry *> &argsTypes) {
    auto functionsWithName = findCandidates(name);

    return Utils::findMatchingFunctions(functionsWithName, argsTypes);
}

FunctionEntry *FunctionsTable::add(FunctionEntry *entry) {
    auto paramsTypes = std::vector<TypeEntry *>();
    for (const auto& param: entry->params) {
        paramsTypes.push_back(param.type);
    }

    auto existingFunction = find(entry->name, paramsTypes);

    if (existingFunction != nullptr) {
        auto it = std::find(functions.begin(), functions.end(), existingFunction);

        if (it != functions.end()) {
            auto index = std::distance(functions.begin(), it);

            functions.erase(functions.begin()+index);
        }
    }

    functions.push_back(entry);

    return entry;
}

std::vector<FunctionEntry *> FunctionsTable::findCandidates(const std::string &name) {
    return Utils::findCandidatesFunctions(functions, name);
}
