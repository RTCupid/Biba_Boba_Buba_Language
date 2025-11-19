#ifndef FRONTEND_INCLUDE_LEXER_HPP
#define FRONTEND_INCLUDE_LEXER_HPP

#include "parser.hpp"
#include <iostream>
#include <ostream>
#include <string>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif

namespace language {

class Lexer : public yyFlexLexer {
  private:
    std::string current_lexem;
    std::string current_value;

  public:
    Lexer(std::istream *in, std::ostream *out) : yyFlexLexer(in, out) {}

    int process_if() {
        current_lexem = "conditional operator";
        current_value = "if";
        return yy::parser::token::TOK_IF;
    }

    int process_else() {
        current_lexem = "conditional operator";
        current_value = "else";
        return yy::parser::token::TOK_ELSE;
    }

    int process_while() {
        current_lexem = "conditional operator";
        current_value = "while";
        return yy::parser::token::TOK_WHILE;
    }

    int process_print() {
        current_lexem = "operator";
        current_value = "print";
        return yy::parser::token::TOK_PRINT;
    }

    int process_input() {
        current_lexem = "operator";
        current_value = "?";
        return yy::parser::token::TOK_INPUT;
    }

    int process_plus() {
        current_lexem = "binary operator";
        current_value = "+";
        return yy::parser::token::TOK_PLUS;
    }

    int process_minus() {
        current_lexem = "binary operator";
        current_value = "-";
        return yy::parser::token::TOK_MINUS;
    }

    int process_mul() {
        current_lexem = "binary operator";
        current_value = "*";
        return yy::parser::token::TOK_MUL;
    }

    int process_rem_div() {
        current_lexem = "binary operator";
        current_value = "%";
        return yy::parser::token::TOK_REM_DIV;
    }

    int process_div() {
        current_lexem = "binary operator";
        current_value = "/";
        return yy::parser::token::TOK_DIV;
    }

    int process_assign() {
        current_lexem = "binary operator";
        current_value = "=";
        return yy::parser::token::TOK_ASSIGN;
    }

    int process_eq() {
        current_lexem = "comparing operator";
        current_value = "==";
        return yy::parser::token::TOK_EQ;
    }

    int process_not_eq() {
        current_lexem = "comparing operator";
        current_value = "!=";
        return yy::parser::token::TOK_NEQ;
    }

    int process_less() {
        current_lexem = "comparing operator";
        current_value = "<";
        return yy::parser::token::TOK_LESS;
    }

    int process_greater() {
        current_lexem = "comparing operator";
        current_value = ">";
        return yy::parser::token::TOK_GREATER;
    }

    int process_less_or_eq() {
        current_lexem = "comparing operator";
        current_value = "<=";
        return yy::parser::token::TOK_LESS_OR_EQ;
    }

    int process_greater_or_eq() {
        current_lexem = "comparing operator";
        current_value = ">=";
        return yy::parser::token::TOK_GREATER_OR_EQ;
    }

    int process_not() {
        current_lexem = "unary operator";
        current_value = "!";
        return yy::parser::token::TOK_NOT;
    }

    int process_left_paren() {
        current_lexem = "identifier";
        current_value = "(";
        return yy::parser::token::TOK_LEFT_PAREN;
    }

    int process_right_paren() {
        current_lexem = "identifier";
        current_value = ")";
        return yy::parser::token::TOK_RIGHT_PAREN;
    }

    int process_left_brace() {
        current_lexem = "identifier";
        current_value = "{";
        return yy::parser::token::TOK_LEFT_BRACE;
    }

    int process_right_brace() {
        current_lexem = "identifier";
        current_value = "}";
        return yy::parser::token::TOK_RIGHT_BRACE;
    }

    int process_semicolon() {
        current_lexem = "identifier";
        current_value = ";";
        return yy::parser::token::TOK_SEMICOLON;
    }

    int process_id() {
        current_lexem = "variable";
        current_value = yytext;
        return yy::parser::token::TOK_ID;
    }

    int process_number() {
        current_lexem = "number";
        current_value = yytext;
        return yy::parser::token::TOK_NUMBER;
    }

    int yylex() override;

    void print_current() const {
        std::cout << current_lexem << " <" << current_value << ">" << std::endl;
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_LEXER_HPP
