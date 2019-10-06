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
    bytes.push_back(new ByteResolver(OP_CONST, nullptr));

    if (auto nativeEntry = dynamic_cast<NativeFunctionEntry *>(entry)) {
        auto v = p2v((void *) nativeEntry->fun);
        auto addr = compiler->registerConst(v);

        bytes.push_back(new ByteResolver(addr, nullptr));
    } else {
        bytes.push_back(new ByteResolver([entry](Compiler *c) {
            auto v = i2v(c->getAddr(entry->firstByte));

            return c->registerConst(v);
        }, nullptr));
    }
}

FunctionEntry *Utils::findMatchingFunctions(
        const std::vector<FunctionEntry *> &functions,
        std::vector<TypeEntry *> argsTypes
) {
    for (auto entry : functions) {
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

    return nullptr;
}

std::vector<TypeEntry *> Utils::getTypes(const std::vector<Expr *>& exprs, Compiler *compiler) {
    auto exprsTypes = std::vector<TypeEntry *>();

    for (auto expr : exprs) {
        auto types = expr->getReturnType(compiler);

        if (!types.single()) {
            throw CompilerError("Return types have to be single to be converted to types");
        }

        exprsTypes.push_back(types[0]);
    }

    return exprsTypes;
}
