//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>

#include <utility>
#include <lib/compiler/CompilerError.h>
#include "FunctionStmt.h"
#include "BlockStmt.h"
#include "lib/compiler/TypeReference.h"

FunctionStmt::FunctionStmt(
        Token *type,
        ParameterDefinition *receiver,
        Token *name,
        std::vector<Token *> returnTypes,
        std::vector<ParameterDefinition> parameters,
        std::vector<Stmt *> body,
        Token *closeBlock
) : type(type), receiver(receiver), name(name), returnTypes(std::move(returnTypes)), parameters(std::move(parameters)),
    body(std::move(body)),
    closeBlock(closeBlock) {

}

FunctionEntry *FunctionStmt::getFunctionEntry(Compiler *compiler) {
    if (_functionEntry != nullptr) {
        return _functionEntry;
    }

    // Get return type
    auto returnTypeReferences = TypeReferences();
    for (auto returnType : returnTypes) {
        auto returnTypeEntry = compiler->frame->findType(returnType->lexeme);

        if (returnTypeEntry == nullptr) {
            throw CompilerError("Cannot find type for " + returnType->lexeme);
        }

        returnTypeReferences.push_back(returnTypeEntry);
    }

    auto entryParameters = std::vector<FunctionEntryParameter>();
    for (auto parameter : parameters) {
        auto paramType = compiler->frame->types.find(parameter.type->lexeme);

        if (paramType == nullptr) {
            throw CompilerError("Cannot find type for " + parameter.type->lexeme);
        }

        entryParameters.emplace_back(parameter.name->lexeme, paramType);
    }

    std::string nameStr;
    if (name != nullptr) {
        nameStr = name->lexeme;
    }

    // Register function
    auto functionEntry = new FunctionEntry(
            nameStr,
            entryParameters,
            returnTypeReferences
    );

    if (autoRegister) {
        if (receiver != nullptr) {
            auto receiverType = compiler->frame->findType(receiver->type->lexeme);

            if (receiverType == nullptr) {
                throw CompilerError("Cannot find type for " + receiver->type->lexeme);
            }

            switch (type->type) {
                case Token::Type::FUNC:
                    functionEntry = receiverType->addFunction(
                            receiver->name->lexeme,
                            receiver->isReference,
                            functionEntry
                    );
                    break;
                case Token::Type::OP:
                    functionEntry = receiverType->addOperator(
                            receiver->name->lexeme,
                            receiver->isReference,
                            functionEntry
                    );
                    break;
                default:
                    throw CompilerError("Unhandled function type");
            }
        } else {
            functionEntry = compiler->frame->functions.add(functionEntry);
        }
    }

    this->_functionEntry = functionEntry;

    return functionEntry;
}

std::vector<ByteResolver *> FunctionStmt::compile(Compiler *compiler) {
    auto functionEntry = getFunctionEntry(compiler);

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

    // Ensure functions have a return TODO: add branches check
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
