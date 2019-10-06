//
// Created by rvigee on 10/4/19.
//

#include "FunctionsTable.h"
#include "TypesTable.h"
#include "lib/compiler/utils/Utils.h"

#include <utility>
#include <sstream>

FunctionEntryParameter::FunctionEntryParameter(std::string name, TypeEntry *type) : name(std::move(name)), type(type) {}

FunctionEntry::FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeEntry *returnType) :
        name(std::move(name)), params(std::move(params)), returnType(returnType) {
    typeEntry = new FunctionTypeEntry(name, this);
}

NativeFunctionEntry::NativeFunctionEntry(
        std::string name,
        std::vector<FunctionEntryParameter> params,
        TypeEntry *returnType,
        Value (*fun)(Value[], int)
) : FunctionEntry(std::move(name), std::move(params), returnType), fun(fun) {

}

FunctionEntry *FunctionsTable::find(const std::string &name, const std::vector<TypeEntry *> &argsTypes) {
    auto functionsWithName = findCandidates(name);

    return Utils::findMatchingFunctions(functionsWithName, argsTypes);
}

FunctionEntry *FunctionsTable::add(FunctionEntry *entry) {
    functions.push_back(entry);

    return entry;
}

std::vector<FunctionEntry *> FunctionsTable::findCandidates(const std::string &name) {
    return Utils::findCandidatesFunctions(functions, name);
}
