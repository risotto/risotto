//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
extern "C" {
#include <lib/vm/value.h>
}

TEST(Value, Size) {
    EXPECT_EQ(sizeof(Value), 16);
}
