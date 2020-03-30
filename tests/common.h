//
// Created by raphael on 21/06/18.
//

#ifndef RISOTTOPROJECT_TESTS_COMMON_H
#define RISOTTOPROJECT_TESTS_COMMON_H

#include <sstream>
#include "lib/Risotto.h"
#include <gtest/gtest.h>

#define INIT_RISOTTO(flags) auto risotto = Risotto(flags)

#define INIT_TEST(flags) INIT_RISOTTO(flags); testing::internal::CaptureStdout()
#define FILE(name) (dir(__FILE__) + "/" + (name))
#define INPUT(name) FILE(name ".rst")
#define OUTPUT(name) get_file_contents(FILE(name ".out"))

#define EXPECT_STDOUT(expected) EXPECT_EQ(::testing::internal::GetCapturedStdout(), expected)
#define R_EXPECT_DEATH(statement, regex) EXPECT_DEATH({INIT_RISOTTO (statement);}, regex);

std::string dir(const std::string& file_path);
std::string get_file_contents(const std::string& filename);

#endif //RISOTTOPROJECT_TESTS_COMMON_H
