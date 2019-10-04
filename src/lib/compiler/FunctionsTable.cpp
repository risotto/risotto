//
// Created by rvigee on 10/4/19.
//

#include "FunctionsTable.h"

#include <utility>

FunctionEntryParameter::FunctionEntryParameter(const std::string &name, TypeEntry *type) : name(name), type(type) {}

FunctionEntry::FunctionEntry(std::string name, std::vector<FunctionEntryParameter> params, TypeEntry *returnType) :
        name(std::move(name)), params(std::move(params)), returnType(returnType) {

}

NativeFunctionEntry::NativeFunctionEntry(
        std::string name,
        std::vector<FunctionEntryParameter> params,
        TypeEntry *returnType,
        Value (*fun)(Value[], int)
) : FunctionEntry(std::move(name), std::move(params), returnType), fun(fun) {

}

FunctionEntry *FunctionsTable::find(const std::string &name) {
    for (auto entry : functions) {
        if (entry->name == name) {
            return entry;
        }
    }

    return nullptr;
}

FunctionEntry *FunctionsTable::add(FunctionEntry *entry) {
    functions.push_back(entry);

    return entry;
}
