//
// Created by rvigee on 10/6/19.
//

#ifndef RISOTTOV2_UTILS_H
#define RISOTTOV2_UTILS_H

#include <vector>
#include <string>
#include <lib/compiler/TypesTable.h>
#include <lib/parser/nodes/Expr.h>

class FunctionEntry;
class ByteResolver;
class Compiler;

class Utils {
public:
    static std::vector<FunctionEntry *> findCandidatesFunctions(const std::vector<FunctionEntry *>& functions, const std::string& name);
    static FunctionEntry * findMatchingFunctions(const std::vector<FunctionEntry *>& functions, std::vector<TypeEntry *> argsTypes);

    static void loadFunctionEntryAddr(Compiler *compiler, FunctionEntry *entry, std::vector<ByteResolver *> &bytes);

    static std::vector<TypeEntry *> getTypes(const std::vector<Expr *>&, Compiler *compiler);
};

#endif //RISOTTOV2_UTILS_H
