//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_NODE_H
#define RISOTTOV2_NODE_H

#include <vector>

class ByteResolver;

class Compiler;

class Node {
public:
    virtual ~Node() = default;

    virtual std::vector<ByteResolver *> compile(Compiler *compiler) = 0;
};


#endif //RISOTTOV2_NODE_H
