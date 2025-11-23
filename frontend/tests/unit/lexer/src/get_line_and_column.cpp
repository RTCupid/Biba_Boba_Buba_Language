#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

#include "lexer.hpp"

using language::Lexer;

// get_line / get_column

TEST(LexerTest, GetLineAndColumnReturnCurrentValues) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    lexer.yylineno = 10;
    lexer.yycolumn = 20;

    EXPECT_EQ(lexer.get_line(), 10);
    EXPECT_EQ(lexer.get_column(), 20);
}