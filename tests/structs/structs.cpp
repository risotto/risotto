//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include "../common.h"

TEST(Structs, Structs) {
    INIT_TEST();

    risotto.runFile(INPUT("structs"));

    EXPECT_STDOUT("1\n2\n3\n");
}
