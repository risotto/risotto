//
// Created by rvigee on 10/4/19.
//

#include "FunctionsTable.h"
#include "TypesTable.h"

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

FunctionEntry *FunctionsTable::find(const std::string &name, std::vector<TypeEntry *> argsTypes) {
    for (auto entry : functions) {
        if (entry->name == name) {
            if (entry->params.size() == argsTypes.size()) {
                auto compatible = true;
                for (int i = 0; i < entry->params.size(); ++i) {
                    auto paramType = entry->params[i].type;
                    auto argType = argsTypes[i];

                    if (!paramType->canReceiveType(argType)) {
                        compatible = false;
                        break;
                    }
                }

                if (compatible) {
                    return entry;
                }
            }
        }
    }

    return nullptr;
}

FunctionEntry *FunctionsTable::add(FunctionEntry *entry) {
    functions.push_back(entry);

    return entry;
}
