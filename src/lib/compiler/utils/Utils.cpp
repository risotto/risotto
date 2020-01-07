//
// Created by rvigee on 10/6/19.
//

extern "C" {
#include <lib/vm/chunk.h>
}

#include <lib/compiler/CompilerError.h>
#include "Utils.h"
#include "lib/compiler/FunctionsTable.h"
#include "lib/compiler/Compiler.h"
#include <lib/parser/nodes/TypeDescriptor.h>
#include <cassert>

std::vector<FunctionEntry *>
Utils::findCandidatesFunctions(const std::vector<FunctionEntry *> &functions, const std::string &name) {
    auto candidates = std::vector<FunctionEntry *>();

    for (auto entry : functions) {
        if (entry->name == name) {
            candidates.push_back(entry);
        }
    }

    return candidates;
}

void Utils::loadFunctionEntryAddr(Compiler *compiler, FunctionEntry *entry, std::vector<ByteResolver *> &bytes) {
    if (auto interfaceEntry = dynamic_cast<DeclarationFunctionEntry *>(entry)) {
        // Load object
        bytes.push_back(new ByteResolver(OP_LOAD_LOCAL, nullptr));
        bytes.push_back(new ByteResolver(0, nullptr)); // frames distance
        bytes.push_back(new ByteResolver(0, nullptr)); // index

        bytes.push_back(new ByteResolver(OP_RESOLVE_ADDR, nullptr));
        bytes.push_back(new ByteResolver(interfaceEntry->addr, nullptr));
    } else if (auto nativeEntry = dynamic_cast<NativeFunctionEntry *>(entry)) {
        bytes.push_back(new ByteResolver(OP_CONST, nullptr));
        bytes.push_back(new ByteResolver([nativeEntry](Compiler *c) {
            auto v = p2v((void *) nativeEntry->fun);

            return c->registerConst(v);
        }, nullptr));
    } else {
        bytes.push_back(new ByteResolver(OP_CONST, nullptr));
        bytes.push_back(new ByteResolver([entry](Compiler *c) {
            auto v = i2v(c->getAddr(entry->firstByte));

            return c->registerConst(v);
        }, nullptr));
    }
}

FunctionEntry *Utils::findMatchingFunctions(
        const std::vector<FunctionEntry *> &functions,
        std::vector<TypeDescriptor *> argsTypes
) {
    for (auto entry : functions) {
        if (entry->descriptor->params.size() == argsTypes.size()) {
            auto compatible = true;
            for (int i = 0; i < entry->descriptor->params.size(); ++i) {
                auto paramType = entry->descriptor->params[i]->type;
                auto argType = argsTypes[i];

                if (!argType->canReceiveType(paramType)) {
                    compatible = false;
                    break;
                }
            }

            if (compatible) {
                return entry;
            }
        }
    }

    return nullptr;
}

bool Utils::typesMatch(
        const std::vector<ParameterDefinition *> &params,
        std::vector<ParameterDefinition *> args
) {
    if (params.size() != args.size()) {
        return false;
    }

    auto paramsTypes = std::vector<TypeDescriptor *>();
    for (const auto &param: params) {
        paramsTypes.push_back(param->type);
    }

    auto argsTypes = std::vector<TypeDescriptor *>();
    for (const auto &arg: args) {
        argsTypes.push_back(arg->type);
    }

    for (int i = 0; i < params.size(); ++i) {
        auto param = params[i];
        auto arg = args[i];

        if (param->asReference != arg->asReference) {
            return false;
        }
    }

    return typesMatch(paramsTypes, argsTypes);
}

bool Utils::typesMatch(const std::vector<TypeDescriptor *> &params, std::vector<TypeDescriptor *> args) {
    if (params.size() != args.size()) {
        return false;
    }

    for (int i = 0; i < params.size(); ++i) {
        auto paramType = params[i];
        auto argType = args[i];

        if (!paramType->canReceiveType(argType)) {
            return false;
        }
    }

    return true;
}

std::vector<TypeDescriptor *> Utils::getTypes(const std::vector<Expr *> &exprs, Compiler *compiler) {
    auto exprsTypes = std::vector<TypeDescriptor *>();

    for (auto expr : exprs) {
        auto types = expr->getReturnType(compiler);

        if (!types.single()) {
            throw CompilerError("Return types have to be single to be converted to types");
        }

        exprsTypes.push_back(types[0]);
    }

    return exprsTypes;
}
