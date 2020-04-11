//
// Created by rvigee on 10/2/19.
//

#include <vector>
#include <sstream>
#include <iomanip>
#include "TokensPrinter.h"

std::string TokensPrinter::print(std::vector<Token> tokens) {
    std::stringstream ss;

    auto n = 3;

    // Header
    ss << std::left << std::setw(n);
    ss << "I";

    ss << std::right << std::setw(n);
    ss << "L";

    ss << " ";

    ss << std::right << std::setw(n);
    ss << "C";

    ss << " : ";

    ss << std::left << std::setw(18);
    ss << "Type";

    ss << "| " << "Lexeme" << std::endl;
    auto separator = "===================================";
    ss << separator << std::endl;

    int previousLine = -1;
    int previousColumn = -1;

    // Content
    auto i = 0;
    for (const auto& token : tokens) {
        ss << std::left << std::setw(n);

        ss << i;

        ss << std::right << std::setw(n);

        if (previousLine != token.position.line) {
            ss << token.position.line;
            previousLine = token.position.line;
            previousColumn = -1;
        } else {
            ss << "|";
        }

        ss << " ";

        ss << std::right << std::setw(n);

        if (previousColumn != token.position.column) {
            ss << token.position.column;
            previousColumn = token.position.column;
        } else {
            ss << "|";
        }

        ss << " : ";

        ss << std::left << std::setw(18);
        ss << wise_enum::to_string(token.type);

        ss << "|";
        if (!token.lexeme.empty() && token.lexeme != "\n") {
            ss << " " << token.lexeme;
        }
        ss << std::endl;

        i++;
    }

    ss << separator << std::endl;

    return ss.str();
}
