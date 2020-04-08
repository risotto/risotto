//
// Created by raphael on 21/06/18.
//

#include <iostream>
#include <fstream>
#include "common.h"

std::string dir(const std::string &file_path) {
    std::string dir_path = file_path.substr(0, file_path.rfind('/'));
    return dir_path;
}

std::string get_file_contents(const std::string &filename) {
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return str;
}

std::string testoutput;

void init_custom_printf() {
    testoutput = std::string("");
}

int custom_printf(const char *fmt, ...) {
    va_list args;

    char buf[256];
    va_start(args, fmt);
    const auto r = std::vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    if (r < 0) {
        // conversion failed
        return r;
    }

    const size_t len = r;

    std::string s(len, '\0');
    va_start(args, fmt);
    std::vsnprintf(s.data(), len + 1, fmt, args);
    va_end(args);

    std::cout << s;
    testoutput += s;

    return r;
}
