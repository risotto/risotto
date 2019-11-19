//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>

#include <utility>
#include <lib/compiler/CompilerError.h>
#include "FunctionStmt.h"
#include "BlockStmt.h"
#include "lib/compiler/TypeDefinition.h"
#include "lib/compiler/ReturnTypes.h"

FunctionStmt::FunctionStmt(
        Token *type,
        ParameterDefinition *receiver,
        Token *name,
        std::vector<TypeDescriptor *> returnTypes,
        std::vector<ParameterDefinition *> parameters,
        std::vector<Stmt *> body,
        Token *closeBlock
) : type(type),
    receiver(receiver),
    name(name),
    returnTypes(std::move(returnTypes)),
    parameters(std::move(parameters)),
    body(std::move(body)),
    closeBlock(closeBlock) {

}

FunctionEntry *FunctionStmt::getFunctionEntry(Compiler *compiler) {
    if (_functionEntry != nullptr) {
        return _functionEntry;
    }

    // Get return type
    auto returnTypeReferences = ReturnTypes();
    for (auto returnType : returnTypes) {
        returnTypeReferences.push_back(returnType);
    }

    std::string nameStr;
    if (name != nullptr) {
        nameStr = name->lexeme;
    }

    // Register function
    auto functionEntry = new FunctionEntry(
            nameStr,
            parameters,
            returnTypeReferences
    );

    if (autoRegister) {
        if (receiver != nullptr) {
            auto receiverType = receiver->type->getTypeDefinition();

            switch (type->type) {
                case TokenType::FUNC:
                    functionEntry = receiverType->addFunction(
                            receiver,
                            functionEntry
                    );
                    break;
                case TokenType::OP:
                    functionEntry = receiverType->addOperator(
                            receiver,
                            functionEntry
                    );
                    break;
                case TokenType::NEW: {
                    auto structType = dynamic_cast<StructTypeDefinition *>(receiverType);
                    if (structType == nullptr) {
                        throw CompilerError("Receiver must be a struct", receiver->name->position);
                    }

                    functionEntry = structType->addConstructor(
                            receiver,
                            functionEntry
                    );
                    break;
                }
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

    compiler->frame = bodyFrame;

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
    compiler->frame = compiler->frame->parent;

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

void FunctionStmt::symbolize(Compiler *compiler) {
    for (auto param: parameters) {
        compiler->linkables.push_back(new LinkUnit(param->type, compiler->frame));
    }

    for (auto returnType: returnTypes) {
        compiler->linkables.push_back(new LinkUnit(returnType, compiler->frame));
    }

    if (receiver != nullptr) {
        compiler->linkables.push_back(new LinkUnit(receiver->type, compiler->frame));
    }

    // Create new frame
    bodyFrame = new Frame(compiler->frame, FUNCTION);

    compiler->frame = bodyFrame;

    if (receiver != nullptr) {
        compiler->frame->variables.add(receiver->name->lexeme, receiver->type);
    }

    // Declare parameters
    for (const auto &param : parameters) {
        compiler->frame->variables.add(param->name->lexeme, param->type);
    }

    for (auto stmt : body) {
        stmt->symbolize(compiler);
    }

    // Restore frame
    compiler->frame = compiler->frame->parent;
}
