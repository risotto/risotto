#include <gtest/gtest.h>
#include "./common.h"

TEST(Benchmark, Bounce) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/bounce"));

    EXPECT_STDOUT("1331\n");
}

TEST(Benchmark, List) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/list"));

    EXPECT_STDOUT("10\n");
}

TEST(Benchmark, Mandelbrot) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/mandelbrot"), {"1"});

    EXPECT_STDOUT("128\n");
}

TEST(Benchmark, Permute) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/permute"));

    EXPECT_STDOUT("8660\n");
}

TEST(Benchmark, Sieve) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/sieve"));

    EXPECT_STDOUT("669\n");
}

TEST(Benchmark, Storage) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/storage"));

    EXPECT_STDOUT("5461\n");
}

TEST(Benchmark, Fib) {
    INIT_TEST;

    risotto.runFile(INPUT("../benchmarks/fib"));

    EXPECT_STDOUT("832040\n");
}
