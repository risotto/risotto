//
// Created by rvigee on 10/2/19.
//

#include <utility>
#include <vector>
#include <iostream>
#include <iomanip>
#include "TokensPrinter.h"

void TokensPrinter::print(std::vector<Token *> tokens) {
    auto n = 3;

    // Header
    std::cout << std::left << std::setw(n);
    std::cout << "I";

    std::cout << std::right << std::setw(n);
    std::cout << "L";

    std::cout << " ";

    std::cout << std::right << std::setw(n);
    std::cout << "C";

    std::cout << " : ";

    std::cout << std::left << std::setw(13);
    std::cout << "Type";

    std::cout << "| " << "Lexeme" << "\n";
    std::cout << "===================================\n";

    int previousLine = -1;
    int previousColumn = -1;

    // Content
    auto i = 0;
    for (auto token : tokens) {
        std::cout << std::left << std::setw(n);

        std::cout << i;

        std::cout << std::right << std::setw(n);

        if (previousLine != token->position.line) {
            std::cout << token->position.line;
            previousLine = token->position.line;
            previousColumn = -1;
        } else {
            std::cout << "|";
        }

        std::cout << " ";

        std::cout << std::right << std::setw(n);

        if (previousColumn != token->position.column) {
            std::cout << token->position.column;
            previousColumn = token->position.column;
        } else {
            std::cout << "|";
        }

        std::cout << " : ";

        std::cout << std::left << std::setw(13);
        std::cout << token->type;

        std::cout << "| " << token->lexeme << "\n";

        i++;
    }
}
