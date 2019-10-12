//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include "../common.h"

TEST(Arrays, BoundFunction) {
    INIT_TEST;

    risotto.runFile(INPUT("bound"));

    EXPECT_STDOUT("YO\n");
}
