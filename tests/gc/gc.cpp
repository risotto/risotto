//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include "../common.h"

TEST(GC, Global) {
    INIT_TEST();

    risotto.runFile(INPUT("gc"));

    EXPECT_STDOUT(OUTPUT("gc"));
}
