//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include "../common.h"

TEST(Interfaces, Interfaces) {
    INIT_TEST;

    risotto.runFile(INPUT("interfaces"));

    EXPECT_STDOUT("Hey! Student\nHello. Teacher\n");
}
