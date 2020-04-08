//
// Created by raphael on 21/06/18.
//

#ifndef RISOTTOPROJECT_TESTS_COMMON_H
#define RISOTTOPROJECT_TESTS_COMMON_H

#include <sstream>
#include "lib/Risotto.h"
#include <gtest/gtest.h>

#define INIT_RISOTTO(flags) init_custom_printf(); auto risotto = Risotto(flags); risotto.printfp = custom_printf

#define INIT_TEST(flags) INIT_RISOTTO(flags)
#define FILE(name) (dir(__FILE__) + "/" + (name))
#define INPUT(name) FILE(name ".rst")
#define OUTPUT(name) get_file_contents(FILE(name ".out"))

#define EXPECT_STDOUT(actual) EXPECT_EQ(testoutput, actual)
#define R_EXPECT_DEATH(statement, regex) EXPECT_DEATH({INIT_RISOTTO (statement);}, regex);

std::string dir(const std::string &file_path);

std::string get_file_contents(const std::string &filename);

extern std::string testoutput;

void init_custom_printf();
int custom_printf(const char *, ...);

#endif //RISOTTOPROJECT_TESTS_COMMON_H
