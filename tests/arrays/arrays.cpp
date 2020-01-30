//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include "../common.h"

TEST(Arrays, Common) {
    INIT_TEST;

    risotto.runFile(INPUT("common"));

    EXPECT_STDOUT(OUTPUT("common"));
}
