#ifndef FRONTEND_INCLUDE_LEXER_HPP
#define FRONTEND_INCLUDE_LEXER_HPP

#include "parser.hpp"
#include <fstream>
#include <iostream>
#include <ostream>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

namespace language {

class Lexer final : public yyFlexLexer {
  public:
    int yylineno = 1;
    int yycolumn = 1;

    Lexer(std::istream *in, std::ostream *out) : yyFlexLexer(in, out) {}

    int get_line() const { return yylineno; }

    int get_column() const { return yycolumn; }
    int get_yyleng() const { return yyleng; }

    int process_if() const { return yy::parser::token::TOK_IF; }
    int process_else() const { return yy::parser::token::TOK_ELSE; }
    int process_while() const { return yy::parser::token::TOK_WHILE; }
    int process_print() const { return yy::parser::token::TOK_PRINT; }
    int process_input() const { return yy::parser::token::TOK_INPUT; }
    int process_plus() const { return yy::parser::token::TOK_PLUS; }
    int process_minus() const { return yy::parser::token::TOK_MINUS; }
    int process_mul() const { return yy::parser::token::TOK_MUL; }
    int process_rem_div() const { return yy::parser::token::TOK_REM_DIV; }
    int process_div() const { return yy::parser::token::TOK_DIV; }
    int process_and() const { return yy::parser::token::TOK_AND; }
    int process_xor() const { return yy::parser::token::TOK_XOR; }
    int process_or() const { return yy::parser::token::TOK_OR; }
    int process_log_or() const { return yy::parser::token::TOK_LOG_OR; }
    int process_log_and() const { return yy::parser::token::TOK_LOG_AND; }
    int process_assign() const { return yy::parser::token::TOK_ASSIGN; }
    int process_eq() const { return yy::parser::token::TOK_EQ; }
    int process_not_eq() const { return yy::parser::token::TOK_NEQ; }
    int process_less() const { return yy::parser::token::TOK_LESS; }
    int process_greater() const { return yy::parser::token::TOK_GREATER; }
    int process_less_or_eq() const { return yy::parser::token::TOK_LESS_OR_EQ; }
    int process_greater_or_eq() const {
        return yy::parser::token::TOK_GREATER_OR_EQ;
    }
    int process_not() const { return yy::parser::token::TOK_NOT; }
    int process_left_paren() const { return yy::parser::token::TOK_LEFT_PAREN; }
    int process_right_paren() const {
        return yy::parser::token::TOK_RIGHT_PAREN;
    }
    int process_left_brace() const { return yy::parser::token::TOK_LEFT_BRACE; }
    int process_right_brace() const {
        return yy::parser::token::TOK_RIGHT_BRACE;
    }
    int process_semicolon() const { return yy::parser::token::TOK_SEMICOLON; }
    int process_id() const { return yy::parser::token::TOK_ID; }
    int process_number() { return yy::parser::token::TOK_NUMBER; }

    int yylex() override;
};

} // namespace language

#endif // FRONTEND_INCLUDE_LEXER_HPP
