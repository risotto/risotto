//
// Created by raphael on 21/06/18.
//

#include <gtest/gtest.h>
#include "../common.h"

#define OPERATOR_INPUT(op) "println(2.0 " #op " 2.0) println(1 " #op " 2)"
#define SHORTHAND_OPERATOR_INPUT(op) "a := 1 println(a) a " #op " 2 println(a) a " #op " 5 println(a)"
#define LOGICAL_OPERATOR_INPUT(op) "println(true " #op " false) println(false " #op " true) println(true " #op " true) println(false " #op " false)"

TEST(Operators, Precedence) {
    INIT_TEST;

    risotto.run("println(2 + 3 % 4 * 5) println(2 + ((3 % 4) * 5)) println(2 + (3 * 5)) println(2 + 15)");

    EXPECT_STDOUT("17\n17\n17\n17\n");
}

TEST(Operators, UnaryPrefixMinus) {
    INIT_TEST;

    risotto.run("println(-2) println(-(-2)) println(- -2) println(-2.0) println(-(-2.0)) println(- -2.0)");

    EXPECT_STDOUT("-2\n2\n2\n-2.000000\n2.000000\n2.000000\n");
}

TEST(Operators, UnaryPrefixMinusMinus) {
    INIT_TEST;

    risotto.run("a := 3 println(--a) println(a) b := 3.0 println(--b) println(b)");

    EXPECT_STDOUT("2\n2\n2.000000\n2.000000\n");
}

TEST(Operators, DISABLED_UnaryPostfixMinusMinus) {
    INIT_TEST;

    risotto.run("var a = 3; print a--; print a; var b = 3.0; print b--; print b;");

    EXPECT_STDOUT("3\n2\n3.000000\n2.000000\n");
}

TEST(Operators, UnaryPrefixBang) {
    INIT_TEST;

    risotto.run("println(!true) println(!false) println(!!true) println(!!false)");

    EXPECT_STDOUT("false\ntrue\ntrue\nfalse\n");
}

TEST(Operators, UnaryPrefixPlusPlus) {
    INIT_TEST;

    risotto.run("a := 3 println(++a) println(a) b := 3.0 println(++b) println(b)");

    EXPECT_STDOUT("4\n4\n4.000000\n4.000000\n");
}

TEST(Operators, DISABLED_UnaryPostfixPlusPlus) {
    INIT_TEST;

    risotto.run("var a = 3; print a++; print a; var b = 3.0; print b++; print b;");

    EXPECT_STDOUT("3\n4\n3.000000\n4.000000\n");
}

TEST(Operators, BinaryPlus) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(+));

    EXPECT_STDOUT("4.000000\n3\n");
}

TEST(Operators, BinaryPlusString) {
    INIT_TEST;

    risotto.runFile(INPUT("plus_string"));

    EXPECT_STDOUT(OUTPUT("plus_string"));
}

TEST(Operators, BinaryMinus) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(-));

    EXPECT_STDOUT("0.000000\n-1\n");
}

TEST(Operators, BinaryDivide) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(/));

    EXPECT_STDOUT("1.000000\n0\n");
}

TEST(Operators, BinaryMultiply) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(*));

    EXPECT_STDOUT("4.000000\n2\n");
}

TEST(Operators, BinaryModulo) {
    INIT_TEST;

    risotto.run("println(10 % 2) println(11 % 2)");

    EXPECT_STDOUT("0\n1\n");
}

TEST(Operators, BinaryLower) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(<));

    EXPECT_STDOUT("false\ntrue\n");
}

TEST(Operators, BinaryLowerEqual) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(<=));

    EXPECT_STDOUT("true\ntrue\n");
}

TEST(Operators, BinaryGreater) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(>));

    EXPECT_STDOUT("false\nfalse\n");
}

TEST(Operators, BinaryGreaterEqual) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(>=));

    EXPECT_STDOUT("true\nfalse\n");
}

TEST(Operators, BinaryEqualEqual) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(==));

    EXPECT_STDOUT("true\nfalse\n");
}

TEST(Operators, BinaryBangEqual) {
    INIT_TEST;

    risotto.run(OPERATOR_INPUT(!=));

    EXPECT_STDOUT("false\ntrue\n");
}

TEST(Operators, PlusEqual) {
    INIT_TEST;

    risotto.run(SHORTHAND_OPERATOR_INPUT(+=));

    EXPECT_STDOUT("1\n3\n8\n");
}

TEST(Operators, MinusEqual) {
    INIT_TEST;

    risotto.run(SHORTHAND_OPERATOR_INPUT(-=));

    EXPECT_STDOUT("1\n-1\n-6\n");
}

TEST(Operators, StarEqual) {
    INIT_TEST;

    risotto.run(SHORTHAND_OPERATOR_INPUT(*=));

    EXPECT_STDOUT("1\n2\n10\n");
}

TEST(Operators, SlashEqual) {
    INIT_TEST;

    risotto.run(SHORTHAND_OPERATOR_INPUT(/=));

    EXPECT_STDOUT("1\n0\n0\n");
}

TEST(Operators, LogicalOr) {
    INIT_TEST;

    risotto.run(LOGICAL_OPERATOR_INPUT(||));

    EXPECT_STDOUT("true\ntrue\ntrue\nfalse\n");
}


TEST(Operators, LogicalAnd) {
    INIT_TEST;

    risotto.run(LOGICAL_OPERATOR_INPUT(&&));

    EXPECT_STDOUT("false\nfalse\ntrue\nfalse\n");
}

TEST(Operators, BangBool) {
    INIT_TEST;

    risotto.run("println(!true)");

    EXPECT_STDOUT("false\n");
}

TEST(Operators, LogicalTestOrder) {
    INIT_TEST;

    risotto.runFile(INPUT("logical_test_order"));

    EXPECT_STDOUT(OUTPUT("logical_test_order"));
}

TEST(Operators, Overloading) {
    INIT_TEST;

    risotto.runFile(INPUT("overloading"));

    EXPECT_STDOUT(OUTPUT("overloading"));
}
