//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>

#include <utility>
#include <lib/compiler/CompilerError.h>
#include "FunctionStmt.h"
#include "BlockStmt.h"

FunctionStmt::FunctionStmt(
        Token *name,
        Token *returnType,
        std::vector<ParameterDefinition> parameters,
        std::vector<Stmt *> body,
        Token *closeBlock
) : name(name), returnType(returnType), parameters(std::move(parameters)), body(std::move(body)),
    closeBlock(closeBlock) {

}

std::vector<ByteResolver *> FunctionStmt::compile(Compiler *compiler) {
    // Get return type
    auto returnTypeEntry = compiler->frame->findType(returnType->lexeme);

    if (returnTypeEntry == nullptr) {
        throw CompilerError("Cannot find type for " + returnType->lexeme);
    }

    auto entryParameters = std::vector<FunctionEntryParameter>();
    for (auto parameter : parameters) {
        auto paramType = compiler->frame->types.find(parameter.type->lexeme);

        if (paramType == nullptr) {
            throw CompilerError("Cannot find type for " + parameter.type->lexeme);
        }

        entryParameters.emplace_back(parameter.name->lexeme, paramType);
    }

    // Register function in frame
    auto functionEntry = compiler->frame->functions.add(
            new FunctionEntry(
                    name->lexeme,
                    entryParameters,
                    returnTypeEntry
            )
    );

    // Keep reference previous frame
    auto previousFrame = compiler->frame;

    // Create new frame
    compiler->frame = new Frame(previousFrame, FUNCTION);

    // Declare parameters
    for (const auto &param : entryParameters) {
        compiler->frame->variables.add(param.name, param.type);
    }

    auto bytes = std::vector<ByteResolver *>();

    for (auto stmt : body) {
        auto stmtBytes = stmt->compile(compiler);
        bytes.insert(bytes.end(), stmtBytes.begin(), stmtBytes.end());
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
