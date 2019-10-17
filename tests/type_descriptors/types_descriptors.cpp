//
// Created by raphael on 21/06/18.
//


#include <gtest/gtest.h>
#include <lib/parser/nodes/TypeDescriptor.h>

TEST(TypesDescriptor, Identifier) {
    auto typeDesc = new IdentifierTypeDescriptor(
            new Token(
                    TokenType::INT,
                    ValueData{._str = "int"},
                    "int",
                    Position(1, 1)
            )
    );

    EXPECT_EQ(typeDesc->toString(), std::string("int"));
}

TEST(TypesDescriptor, Array) {
    auto typeDesc = new ArrayTypeDescriptor(
            new IdentifierTypeDescriptor(
                    new Token(
                            TokenType::INT,
                            ValueData{._str = "int"},
                            "int",
                            Position(1, 1)
                    )
            )
    );

    EXPECT_EQ(typeDesc->toString(), std::string("[]int"));
}

TEST(TypesDescriptor, Struct) {
    auto intDesc = new IdentifierTypeDescriptor(
            new Token(
                    TokenType::INT,
                    ValueData{._str = "int"},
                    "int",
                    Position(1, 1)
            )
    );

    auto typeDesc = new StructTypeDescriptor(
            {
                    StructTypeDescriptor::Field(new Token(
                            TokenType::IDENTIFIER,
                            ValueData{._str = "field1"},
                            "field1",
                            Position(1, 1)
                    ), intDesc),
                    StructTypeDescriptor::Field(new Token(
                            TokenType::IDENTIFIER,
                            ValueData{._str = "field2"},
                            "field2",
                            Position(1, 1)
                    ), intDesc),
            }
    );

    EXPECT_EQ(typeDesc->toString(), std::string("struct { field1 int; field2 int; }"));
}

TEST(TypesDescriptor, Function) {
    auto intDesc = new IdentifierTypeDescriptor(
            new Token(
                    TokenType::INT,
                    ValueData{._str = "int"},
                    "int",
                    Position(1, 1)
            )
    );

    auto typeDesc = new FunctionTypeDescriptor(
            {
                    ParameterDefinition(
                            new Token(
                                    TokenType::IDENTIFIER,
                                    ValueData{._str = "var1"},
                                    "var1",
                                    Position(1, 1)
                            ),
                            intDesc,
                            true
                    ),
                    ParameterDefinition(
                            new Token(
                                    TokenType::IDENTIFIER,
                                    ValueData{._str = "var2"},
                                    "var2",
                                    Position(1, 1)
                            ),
                            intDesc,
                            true
                    ),
            },
            {
                    intDesc,
                    intDesc,
            }
    );

    EXPECT_EQ(typeDesc->toString(), std::string("func (var1 int, var2 int) (int, int)"));
}
