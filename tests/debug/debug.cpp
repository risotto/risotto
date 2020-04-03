//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include <gtest/gtest-matchers.h>
#include "../common.h"

TEST(Debug, Tokens) {
    INIT_TEST(PrintTokens);

    risotto.runFile(INPUT("test"));

    EXPECT_STDOUT(OUTPUT("tokens"));
}

TEST(Debug, AST) {
    INIT_TEST(PrintAST);

    risotto.runFile(INPUT("test"));

    EXPECT_STDOUT(OUTPUT("ast"));
}

TEST(Debug, Disassembled) {
    INIT_TEST(PrintDisassembled);

    risotto.runFile(INPUT("test"));

    EXPECT_STDOUT(OUTPUT("disassembled"));
}

TEST(Debug, Trace) {
    INIT_TEST(PrintTraceExecution);

    risotto.runFile(INPUT("test"));

    EXPECT_STDOUT(OUTPUT("trace"));
}
