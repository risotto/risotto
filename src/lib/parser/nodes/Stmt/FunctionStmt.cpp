//
// Created by rvigee on 10/2/19.
//

#include <lib/compiler/Compiler.h>

#include <utility>
#include <lib/compiler/CompilerError.h>
#include <cassert>
#include "FunctionStmt.h"
#include "BlockStmt.h"
#include "lib/compiler/TypeDefinition.h"
#include "lib/compiler/ReturnTypes.h"

FunctionStmt::FunctionStmt(
        Token *type,
        ParameterDefinition *receiver,
        Token *name,
        const std::vector<TypeDescriptor *> &returnTypes,
        const std::vector<ParameterDefinition *> &parameters,
        std::vector<Stmt *> body,
        Token *closeBlock
) : type(type),
    receiver(receiver),
    name(name),
    returnTypes(returnTypes),
    parameters(parameters),
    body(std::move(body)),
    closeBlock(closeBlock) {
    descriptor = new FunctionTypeDescriptor(parameters, returnTypes);
}

FunctionEntry *FunctionStmt::getFunctionEntry(Compiler *compiler) {
    return _functionEntry;
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
    if (functionEntry->descriptor->returnTypes.empty()) {
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
    compiler->typesManager->addListener([this, compiler]() {
        return registerFunction(compiler);
    });

    compiler->typesManager->add(descriptor, compiler->frame);

    if (receiver != nullptr) {
        compiler->typesManager->add(receiver->type, compiler->frame);
    }

    for (auto param: parameters) {
        compiler->typesManager->add(param->type, compiler->frame);
    }

    for (auto returnType: returnTypes) {
        compiler->typesManager->add(returnType, compiler->frame);
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

bool FunctionStmt::registerFunction(Compiler *compiler) {
    std::string nameStr;
    if (name != nullptr) {
        nameStr = name->lexeme;
    }

    _functionEntry = new FunctionEntry(nameStr, descriptor);

    if (autoRegister) {
        if (receiver != nullptr) {
            auto receiverType = receiver->type->getTypeDefinition();

            if (receiverType == nullptr) {
                return false;
            }

            switch (type->type) {
                case TokenType::FUNC:
                    _functionEntry = receiverType->addFunction(
                            receiver,
                            _functionEntry
                    );
                    break;
                case TokenType::OP:
                    _functionEntry = receiverType->addOperator(
                            receiver,
                            _functionEntry
                    );
                    break;
                case TokenType::NEW: {
                    auto structType = dynamic_cast<StructTypeDefinition *>(receiverType);
                    if (structType == nullptr) {
                        throw CompilerError("Receiver must be a struct", receiver->name->position);
                    }

                    _functionEntry = structType->addConstructor(
                            receiver,
                            _functionEntry
                    );
                    break;
                }
                default:
                    throw CompilerError("Unhandled function type");
            }
        } else {
            _functionEntry = compiler->frame->functions.add(_functionEntry);
        }
    }

    return true;
}
