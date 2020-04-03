//
// Created by raphael on 21/06/18.
//

#include "../common.h"

TEST(Flow, IfStmt) {
    INIT_TEST();

    risotto.runFile(INPUT("if"));

    EXPECT_STDOUT(OUTPUT("if"));
}

TEST(Flow, WhileStmt) {
    INIT_TEST();

    risotto.runFile(INPUT("while"));

    EXPECT_STDOUT(OUTPUT("while"));
}
