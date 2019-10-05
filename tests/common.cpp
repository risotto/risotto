//
// Created by raphael on 21/06/18.
//

#include <iostream>
#include <fstream>

std::string dir(const std::string &file_path) {
    std::string dir_path = file_path.substr(0, file_path.rfind('/'));
    return dir_path;
}

std::string get_file_contents(const std::string &filename) {
    std::ifstream ifs(filename);
    std::string str((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return str;
}