//
// Created by raphael on 21/06/18.
//

#include <gtest/gtest.h>
#include "../common.h"

#define BIT_EXPR(op) "println(60 " #op " 13)"

TEST(Bit, AND) {
    INIT_TEST();

    risotto.run(BIT_EXPR(&));

    EXPECT_STDOUT("12\n");
}

TEST(Bit, OR) {
    INIT_TEST();

    risotto.run(BIT_EXPR(|));

    EXPECT_STDOUT("61\n");
}

TEST(Bit, XOR) {
    INIT_TEST();

    risotto.run(BIT_EXPR(^));

    EXPECT_STDOUT("49\n");
}

TEST(Bit, Complement) {
    INIT_TEST();

    risotto.run("println(~60)");

    EXPECT_STDOUT("-61\n");
}

TEST(Bit, LeftShift) {
    INIT_TEST();

    risotto.run("println(60 << 2)");

    EXPECT_STDOUT("240\n");
}

TEST(Bit, RightShift) {
    INIT_TEST();

    risotto.run("println(60 >> 2)");

    EXPECT_STDOUT("15\n");
}
