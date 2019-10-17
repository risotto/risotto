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

TEST(Functions, Arguments) {
    INIT_TEST;

    risotto.runFile(INPUT("arguments"));

    EXPECT_STDOUT(OUTPUT("arguments"));
}

TEST(Functions, Bound) {
    INIT_TEST;

    risotto.runFile(INPUT("bound"));

    EXPECT_STDOUT(OUTPUT("bound"));
}

TEST(Functions, Lambda) {
    INIT_TEST;

    risotto.runFile(INPUT("lambda"));

    EXPECT_STDOUT(OUTPUT("lambda"));
}

TEST(Functions, MultipleReturns) {
    INIT_TEST;

    risotto.runFile(INPUT("multiple_returns"));

    EXPECT_STDOUT("1\nyolo\ntrue\n");
}

TEST(Functions, NonExistingFunction) {
    INIT_RISOTTO

    try {
        risotto.run("I_do_not_exist(1, 1.1, 1.2)");
        FAIL() << "Expected CompilerError";
    } catch (CompilerError const &err) {
        EXPECT_EQ(err.what(), std::string("Undefined function: I_do_not_exist(int, double, double) at 1:27"));
    } catch (...) {
        FAIL() << "Expected CompilerError";
    }
}

TEST(Functions, NonExistingBoundFunction) {
    INIT_RISOTTO

    try {
        risotto.run("1.xxx(1.2)");
        FAIL() << "Expected CompilerError";
    } catch (CompilerError const &err) {
        EXPECT_EQ(err.what(), std::string("Undefined function: int.xxx(double) at 1:3"));
    } catch (...) {
        FAIL() << "Expected CompilerError";
    }
}

TEST(Functions, NonExistingOperator) {
    INIT_RISOTTO

    try {
        risotto.run("1 * \"hello\"");
        FAIL() << "Expected CompilerError";
    } catch (CompilerError const &err) {
        EXPECT_EQ(err.what(), std::string("Undefined function: int * string at 1:3"));
    } catch (...) {
        FAIL() << "Expected CompilerError";
    }
}
