#include <iostream>
#include <lib/tokenizer/Tokenizer.h>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
    auto path = argv[1];

    std::ifstream ifs(path, std::ios::in | std::ios::binary | std::ios::ate);

    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);

    auto str = std::string(bytes.data(), fileSize);

    auto tokenizer = new Tokenizer(str);

    auto tokens = tokenizer->tokenize();

    return 0;
}
