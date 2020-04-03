//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>

extern "C" {
#include <lib/vm/utils.h>
}

TEST(Utils, Pack8) {
    bool bools[8] = {1, 1, 0, 1, 0, 0, 0, 1};

    auto n = pack8b(bools);

    EXPECT_EQ(n, 209);
}

TEST(Utils, Unpack8) {
    bool bools[8];

    unpack8b(bools, 209);

    bool expected[] = {1, 1, 0, 1, 0, 0, 0, 1};

    EXPECT_TRUE(0 == memcmp(expected, bools, sizeof(expected)));
}

TEST(Utils, Pack64) {
    bool bools[64] = {0};

    // [1, 0 .... 1, 1, 0, 1, 0, 0, 0, 1]
    bool data[] = {1, 1, 0, 1, 0, 0, 0, 1};
    bools[0] = 1;
    for (int i = 0; i < 8; ++i) {
        bools[64 - 8 + i] = data[i];
    }

    auto n = pack64b(bools);

    EXPECT_EQ(n, 9223372036854776017U);
}

TEST(Utils, Unpack64) {
    bool bools[64];
    unpack64b(bools, 9223372036854776017U);

    EXPECT_EQ(bools[0], 1);
    bool data[] = {1, 1, 0, 1, 0, 0, 0, 1};
    for (int i = 0; i < 8; ++i) {
        EXPECT_EQ(data[i], bools[64 - 8 + i]);
    }
}

TEST(Utils, Max) {
    uint64_t max = (uint64_t) std::numeric_limits<uint64_t>::max;

    bool bools[64];
    unpack64b(bools, max);

    auto n = pack64b(bools);

    EXPECT_EQ(n, max);
}
