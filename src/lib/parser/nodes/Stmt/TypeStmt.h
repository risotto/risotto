//
// Created by rvigee on 10/13/19.
//

#ifndef RISOTTOV2_TYPESTMT_H
#define RISOTTOV2_TYPESTMT_H


#include <lib/parser/nodes/Stmt.h>
#include <lib/parser/nodes/TypeDescriptor.h>

class TypeStmt: public Stmt {
public:
    Token *name;
    TypeDescriptor *typeDescriptor;

    TypeStmt(Token *name, TypeDescriptor *typeDescriptor);

    std::vector<ByteResolver *> compile(Compiler *compiler) override;

    void symbolize(Compiler *compiler) override;

};


#endif //RISOTTOV2_TYPESTMT_H
