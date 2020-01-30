//
// Created by rvigee on 10/6/19.
//

extern "C" {
#include <lib/vm/chunk.h>
#include <lib/vm/value.h>
}

#include <lib/compiler/CompilerError.h>
#include "Utils.h"
#include "lib/compiler/FunctionsTable.h"
#include "lib/compiler/Compiler.h"
#include <lib/parser/nodes/TypeDescriptor.h>
#include <cassert>
#include <utility>

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

bool Utils::loadFunctionEntryAddr(Compiler *compiler, FunctionEntry *entry, std::vector<ByteResolver *> &bytes) {
    if (auto bytesEntry = dynamic_cast<BytesFunctionEntry *>(entry)) {
        auto genBytes = bytesEntry->get();

        bytes.insert(bytes.end(), genBytes.begin(), genBytes.end());

        return true;
    }

    if (auto interfaceEntry = dynamic_cast<DeclarationFunctionEntry *>(entry)) {
        // Load object
        bytes.push_back(new ByteResolver(OP_LOAD_STACK, TODO_POSITION));
        bytes.push_back(new ByteResolver(0));

        bytes.push_back(new ByteResolver(OP_RESOLVE_ADDR));
        auto vaddr = interfaceEntry->addr;
        bytes.push_back(new ByteResolver(vaddr));
    } else if (auto nativeEntry = dynamic_cast<NativeFunctionEntry *>(entry)) {
        bytes.push_back(new ByteResolver(OP_CONST, TODO_POSITION));
        bytes.push_back(new ByteResolver([nativeEntry](Compiler *c) {
            auto v = p2v((void *) nativeEntry->fun);

            return c->registerConst(v);
        }));
    } else if (auto codeEntry = dynamic_cast<CodeFunctionEntry *>(entry)) {
        bytes.push_back(new ByteResolver(OP_CONST, TODO_POSITION));
        bytes.push_back(new ByteResolver([codeEntry](Compiler *c) {
            auto v = i2v(c->getAddr(codeEntry->firstByte));

            return c->registerConst(v);
        }));
    } else {
        throw std::logic_error("Unhandled function entry");
    }

    return false;
}

FunctionEntry *Utils::findMatchingFunctions(
        const std::vector<FunctionEntry *> &functions,
        std::vector<TypeDescriptor *> argsTypes
) {
    for (auto entry : functions) {
        if (entry->descriptor->params.size() == argsTypes.size()) {
            auto compatible = true;
            for (auto i = 0u; i < entry->descriptor->params.size(); ++i) {
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
        std::vector<ParameterDefinition *> &params,
        std::vector<ParameterDefinition *> args,
        const std::function<bool(int i, TypeDescriptor *, TypeDescriptor *)> &comparator
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

    for (auto i = 0u; i < params.size(); ++i) {
        auto param = params[i];
        auto arg = args[i];

        if (param->asReference != arg->asReference) {
            return false;
        }
    }

    return typesMatch(paramsTypes, argsTypes, comparator);
}

bool Utils::typesMatch(
        std::vector<TypeDescriptor *> &params,
        std::vector<TypeDescriptor *> args,
        const std::function<bool(int i, TypeDescriptor *, TypeDescriptor *)> &comparator
) {
    if (params.size() != args.size()) {
        return false;
    }

    for (auto i = 0u; i < params.size(); ++i) {
        auto paramType = params[i];
        auto argType = args[i];

        if (!comparator(i, paramType, argType)) {
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

bool Utils::TypesCompatible(int i, TypeDescriptor *l, TypeDescriptor *r) {
    return l->canReceiveType(r);
}

bool Utils::TypesSame(int i, TypeDescriptor *l, TypeDescriptor *r) {
    return l->isSame(r);
}
