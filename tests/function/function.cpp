//
// Created by raphael on 21/06/18.
//

#include "../common.h"
#include <gtest/gtest.h>
#include <lib/compiler/CompilerError.h>

TEST(Functions, Fib) {
    INIT_TEST;

    risotto.runFile(INPUT("fib"));

    EXPECT_STDOUT("75025\n");
}

TEST(Functions, NonExistingFunction) {
    INIT_RISOTTO

    try {
        risotto.run("I_do_not_exist(1, 1.1, 1.2)");
        FAIL() << "Expected CompilerError";
    } catch(CompilerError const & err) {
        EXPECT_EQ(err.what(),std::string("Cannot find function matching I_do_not_exist(int, double, double) at 1:1"));
    } catch(...) {
        FAIL() << "Expected CompilerError";
    }
}
