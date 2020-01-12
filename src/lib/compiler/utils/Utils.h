//
// Created by rvigee on 10/6/19.
//

#ifndef RISOTTOV2_UTILS_H
#define RISOTTOV2_UTILS_H

#include <vector>
#include <string>
#include <functional>
#include <lib/compiler/TypesTable.h>
#include <lib/parser/nodes/Expr.h>
#include <lib/compiler/FunctionsTable.h>

class FunctionEntry;
class ByteResolver;
class Compiler;

class Utils {
public:
    static std::vector<FunctionEntry *> findCandidatesFunctions(const std::vector<FunctionEntry *>& functions, const std::string& name);
    static FunctionEntry * findMatchingFunctions(const std::vector<FunctionEntry *>& functions, std::vector<TypeDescriptor *> argsTypes);
    static bool typesMatch(std::vector<TypeDescriptor *>& receivers, std::vector<TypeDescriptor *> args, const std::function<bool(int i, TypeDescriptor *, TypeDescriptor *)>& comparator);
    static bool typesMatch(std::vector<ParameterDefinition *>& params, std::vector<ParameterDefinition *> args, const std::function<bool(int i, TypeDescriptor *, TypeDescriptor *)>& comparator);

    static bool loadFunctionEntryAddr(Compiler *compiler, FunctionEntry *entry, std::vector<ByteResolver *> &bytes);

    static std::vector<TypeDescriptor *> getTypes(const std::vector<Expr *>&, Compiler *compiler);

    static bool TypesCompatible(int i, TypeDescriptor *, TypeDescriptor *);
    static bool TypesSame(int i, TypeDescriptor *, TypeDescriptor *);
};

#endif //RISOTTOV2_UTILS_H
