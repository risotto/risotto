//
// Created by rvigee on 10/2/19.
//


#include <lib/compiler/utils/Utils.h>

#include <utility>
#include "BinaryExpr.h"
#include "lib/compiler/Compiler.h"

BinaryExpr::BinaryExpr(Expr *left, Token *op, Expr *right) : GetCallExpr(left, op, op, op, {right}) {
}

Expr *BinaryExpr::left() {
    return callee;
}

Expr *BinaryExpr::right() {
    return args[0];
}

Token *BinaryExpr::op() {
    return rParen;
}

const std::vector<Shortcut> shortcuts = {
        Shortcut([](Compiler *compiler, BinaryExpr *b){
            auto lrt = b->left()->getReturnType(compiler)[0];
            auto rrt = b->right()->getReturnType(compiler)[0];

            if (lrt->isSame(&NilTypeDescriptor::Def) || rrt->isSame(&NilTypeDescriptor::Def)) {
                return b->op()->type == EQUAL_EQUAL || b->op()->type == BANG_EQUAL;
            }

            return false;
        }, [](Compiler *compiler, BinaryExpr *b, std::vector<ByteResolver *> &bytes){
            auto op = b->op();

            auto rrt = b->right()->getReturnType(compiler)[0];
            auto rn = rrt->isSame(&NilTypeDescriptor::Def);

            auto args = b->getArguments(compiler);

            if (rn) { // Load left
                auto argBytes = b->left()->compile(compiler);
                bytes.insert(bytes.end(), argBytes.begin(), argBytes.end());
            } else { // Load right
                auto argBytes = b->right()->compile(compiler);
                bytes.insert(bytes.end(), argBytes.begin(), argBytes.end());
            }

            switch (op->type) {
                case EQUAL_EQUAL:
                    bytes.push_back(new ByteResolver(OP_EQ_NIL, &op->position));
                    return;
                case BANG_EQUAL:
                    bytes.push_back(new ByteResolver(OP_NEQ_NIL, &op->position));
                    return;
                default:
                    throw std::logic_error("unhandled");
            }
        }, [](Compiler *compiler, BinaryExpr *b){
            return compiler->frame->findNamedType("bool");
        })
};

FunctionEntry *BinaryExpr::getFunctionEntry(Compiler *compiler) {
    auto leftReturnType = left()->getReturnType(compiler);

    if (!leftReturnType.single()) {
        throw CompilerError("LHS of binary operation must be a single value", op()->position);
    }

    return leftReturnType[0]->getTypeDefinition()->operators.find(op()->lexeme, getArgumentsTypes(compiler));
}

FunctionNotFoundError BinaryExpr::getFunctionNotFoundError(Compiler *compiler) {
    auto leftReturnType = left()->getReturnType(compiler);
    auto rightReturnType = right()->getReturnType(compiler);
    auto args = getArgumentsTypes(compiler);

    return FunctionNotFoundError(
            leftReturnType[0]->toString() + " " + op()->lexeme + " " + rightReturnType[0]->toString(),
            args,
            op()
    );
}

VariableEntry *BinaryExpr::getVariableEntry(Compiler *compiler) {
    return nullptr;
}

ReturnTypes BinaryExpr::computeReturnType(Compiler *compiler) {
    for (const auto& s: shortcuts) {
        if (s.applies(compiler, this)) {
            return s.returnTypeGen(compiler, this);
        }
    }

    return MixedCallExpr::computeReturnType(compiler);
}

std::vector<ByteResolver *> BinaryExpr::compile(Compiler *compiler) {
    for (const auto& s: shortcuts) {
        if (s.applies(compiler, this)) {
            auto bytes = std::vector<ByteResolver *>();

            s.bytesGen(compiler, this, bytes);

            return bytes;
        }
    }

    return BaseCallExpr::compile(compiler);
}

Shortcut::Shortcut(
        std::function<bool(Compiler *, BinaryExpr *)> applies,
        std::function<void(Compiler *, BinaryExpr *, std::vector<ByteResolver *> &bytes)> bytesGen,
        std::function<TypeDescriptor *(Compiler *, BinaryExpr *)> returnTypeGen
) : applies(std::move(applies)), bytesGen(std::move(bytesGen)), returnTypeGen(std::move(returnTypeGen)) {}
