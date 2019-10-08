//
// Created by rvigee on 10/4/19.
//

#include "FunctionsTable.h"
#include "TypesTable.h"
#include "lib/compiler/utils/Utils.h"

#include <utility>

FunctionEntryParameter::FunctionEntryParameter(std::string name, TypeReference type) : FunctionEntryParameter(std::move(name), type, false) {}
FunctionEntryParameter::FunctionEntryParameter(std::string name, TypeReference type, bool asReference) : name(std::move(name)), type(type), asReference(asReference) {}

FunctionEntry::FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeReferences returnTypes) :
        name(std::move(name)), params(std::move(params)), returnTypes(std::move(returnTypes)) {
    typeEntry = new FunctionTypeEntry(name, this);
}

NativeFunctionEntry::NativeFunctionEntry(
        std::string name,
        std::vector<FunctionEntryParameter> params,
        TypeReferences returnTypes,
        NativeFunctionReturn (*fun)(Value[], int)
) : FunctionEntry(std::move(name), std::move(params), std::move(returnTypes)), fun(fun) {

}

FunctionEntry *FunctionsTable::find(const std::string &name, const std::vector<TypeReference > &argsTypes) {
    auto functionsWithName = findCandidates(name);

    return Utils::findMatchingFunctions(functionsWithName, argsTypes);
}

FunctionEntry *FunctionsTable::add(FunctionEntry *entry) {
    auto paramsTypes = std::vector<TypeReference>();
    for (const auto& param: entry->params) {
        paramsTypes.push_back(param.type);
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
