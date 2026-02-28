#include <gtest/gtest.h>
#include <iostream>
#include <sstream>

#include "parser/lexer.hpp"

using language::Lexer;

int yyFlexLexer::yywrap() { return 1; }

// if
TEST(LexerTest, ProcessIfSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_if();
    EXPECT_EQ(token, yy::parser::token::TOK_IF);
}

// else
TEST(LexerTest, ProcessElseSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_else();
    EXPECT_EQ(token, yy::parser::token::TOK_ELSE);
}

// while
TEST(LexerTest, ProcessWhileSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_while();
    EXPECT_EQ(token, yy::parser::token::TOK_WHILE);
}

// print
TEST(LexerTest, ProcessPrintSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_print();
    EXPECT_EQ(token, yy::parser::token::TOK_PRINT);
}

// input
TEST(LexerTest, ProcessInputSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_input();
    EXPECT_EQ(token, yy::parser::token::TOK_INPUT);
}

// +
TEST(LexerTest, ProcessPlusSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_plus();
    EXPECT_EQ(token, yy::parser::token::TOK_PLUS);
}

// -
TEST(LexerTest, ProcessMinusSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_minus();
    EXPECT_EQ(token, yy::parser::token::TOK_MINUS);
}

// *
TEST(LexerTest, ProcessMulSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_mul();
    EXPECT_EQ(token, yy::parser::token::TOK_MUL);
}

// %
TEST(LexerTest, ProcessRemDivSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_rem_div();
    EXPECT_EQ(token, yy::parser::token::TOK_REM_DIV);
}

// /
TEST(LexerTest, ProcessDivSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_div();
    EXPECT_EQ(token, yy::parser::token::TOK_DIV);
}

// =
TEST(LexerTest, ProcessAssignSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_assign();
    EXPECT_EQ(token, yy::parser::token::TOK_ASSIGN);
}

// ==
TEST(LexerTest, ProcessEqSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_eq();
    EXPECT_EQ(token, yy::parser::token::TOK_EQ);
}

// !=
TEST(LexerTest, ProcessNotEqSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_not_eq();
    EXPECT_EQ(token, yy::parser::token::TOK_NEQ);
}

// <
TEST(LexerTest, ProcessLessSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_less();
    EXPECT_EQ(token, yy::parser::token::TOK_LESS);
}

// >
TEST(LexerTest, ProcessGreaterSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_greater();
    EXPECT_EQ(token, yy::parser::token::TOK_GREATER);
}

// <=
TEST(LexerTest, ProcessLessOrEqSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_less_or_eq();
    EXPECT_EQ(token, yy::parser::token::TOK_LESS_OR_EQ);
}

// >=
TEST(LexerTest, ProcessGreaterOrEqSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_greater_or_eq();
    EXPECT_EQ(token, yy::parser::token::TOK_GREATER_OR_EQ);
}

// !
TEST(LexerTest, ProcessNotSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_not();
    EXPECT_EQ(token, yy::parser::token::TOK_NOT);
}

// (
TEST(LexerTest, ProcessLeftParenSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_left_paren();
    EXPECT_EQ(token, yy::parser::token::TOK_LEFT_PAREN);
}

// )
TEST(LexerTest, ProcessRightParenSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_right_paren();
    EXPECT_EQ(token, yy::parser::token::TOK_RIGHT_PAREN);
}

// {
TEST(LexerTest, ProcessLeftBraceSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_left_brace();
    EXPECT_EQ(token, yy::parser::token::TOK_LEFT_BRACE);
}

// }
TEST(LexerTest, ProcessRightBraceSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_right_brace();
    EXPECT_EQ(token, yy::parser::token::TOK_RIGHT_BRACE);
}

// ;
TEST(LexerTest, ProcessSemicolonSetsToken) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.process_semicolon();
    EXPECT_EQ(token, yy::parser::token::TOK_SEMICOLON);
}

TEST(LexerTest, YyLexIsCallableOnEmptyInput) {
    std::istringstream in("");
    std::ostringstream out;
    Lexer lexer(&in, &out);

    int token = lexer.yylex();
    (void)token;
}
