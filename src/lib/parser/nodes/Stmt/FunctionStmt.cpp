//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>

#include <utility>
#include <lib/compiler/CompilerError.h>
#include "FunctionStmt.h"
#include "BlockStmt.h"

FunctionStmt::FunctionStmt(
        ParameterDefinition *receiver,
        Token *name,
        const std::vector<Token *> &returnTypes,
        std::vector<ParameterDefinition> parameters,
        std::vector<Stmt *> body,
        Token *closeBlock
) : receiver(receiver), name(name), returnTypes(returnTypes), parameters(std::move(parameters)), body(std::move(body)),
    closeBlock(closeBlock) {

}

std::vector<ByteResolver *> FunctionStmt::compile(Compiler *compiler) {
    // Get return type
    auto returnTypesEntries = TypesEntries();
    for (auto returnType : returnTypes) {
        auto returnTypeEntry = compiler->frame->findType(returnType->lexeme);
        returnTypesEntries.push_back(returnTypeEntry);

        if (returnTypeEntry == nullptr) {
            throw CompilerError("Cannot find type for " + returnType->lexeme);
        }
    }

    auto entryParameters = std::vector<FunctionEntryParameter>();
    for (auto parameter : parameters) {
        auto paramType = compiler->frame->types.find(parameter.type->lexeme);

        if (paramType == nullptr) {
            throw CompilerError("Cannot find type for " + parameter.type->lexeme);
        }

        entryParameters.emplace_back(parameter.name->lexeme, paramType);
    }

    // Register function
    auto functionEntry = new FunctionEntry(
            name->lexeme,
            entryParameters,
            returnTypesEntries
    );

    if (receiver != nullptr) {
        auto receiverType = compiler->frame->findType(receiver->type->lexeme);

        if (receiverType == nullptr) {
            throw CompilerError("Cannot find type for " + receiver->type->lexeme);
        }

        functionEntry = receiverType->addFunction(
                receiver->name->lexeme,
                functionEntry
        );
    } else {
        functionEntry = compiler->frame->functions.add(functionEntry);
    }

    // Keep reference previous frame
    auto previousFrame = compiler->frame;

    // Create new frame
    compiler->frame = new Frame(previousFrame, FUNCTION);

    // Declare parameters
    for (const auto &param : functionEntry->params) {
        compiler->frame->variables.add(param.name, param.type);
    }

    auto bytes = std::vector<ByteResolver *>();

    for (auto stmt : body) {
        auto stmtBytes = stmt->compile(compiler);
        bytes.insert(bytes.end(), stmtBytes.begin(), stmtBytes.end());
    }

    if (functionEntry->returnTypes.empty()) {
        bytes.push_back(new ByteResolver(OP_RETURN, nullptr));
        bytes.push_back(new ByteResolver(0, nullptr)); // no frame to drop
        bytes.push_back(new ByteResolver(0, nullptr)); // no value to return
    }

    // Restore frame
    compiler->frame = previousFrame;

    // Save entrypoint of functions
    functionEntry->firstByte = bytes.front();

    // Skip function body
    auto lastByte = bytes.back();
    bytes.insert(bytes.begin(), {
            new ByteResolver(OP_JUMP, nullptr),
            new ByteResolver([lastByte](Compiler *c) {
                return c->getAddr(lastByte) + 1;
            }, nullptr)
    });

    return bytes;
}
