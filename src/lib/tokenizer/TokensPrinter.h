//
// Created by rvigee on 10/2/19.
//

#ifndef RISOTTOV2_TOKENSPRINTER_H
#define RISOTTOV2_TOKENSPRINTER_H


#include "Token.h"

class TokensPrinter {
private:
    std::vector<Token *> tokens;

    int previousLine = -1;
    int previousColumn = -1;

public:
    TokensPrinter(std::vector<Token *> tokens);

    void print();
};


#endif //RISOTTOV2_TOKENSPRINTER_H
