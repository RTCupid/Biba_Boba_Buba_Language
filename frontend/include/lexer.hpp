#ifndef FRONTEND_INCLUDE_LEXER_HPP
#define FRONTEND_INCLUDE_LEXER_HPP

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif
#include <iostream>

namespace language {

class Lexer : public yyFlexLexer {
    std::string current_lexem;
    std::string current_value;

    int process_if() {
        current_lexem = "conditional operator";
        current_value = "if";
        return 1;
    }

    int process_else() {
        current_lexem = "conditional operator";
        current_value = "else";
        return 1;
    }

    int process_while() {
        current_lexem = "conditional operator";
        current_value = "while";
        return 1;
    }

    int process_print() {
        current_lexem = "operator";
        current_value = "print";
        return 1;
    }

    int process_input() {
        current_lexem = "operator";
        current_value = "?";
        return 1;
    }

    int process_plus() {
        current_lexem = "binary operator";
        current_value = "+";
        return 1;
    }

    int process_minus() {
        current_lexem = "binary operator";
        current_value = "-";
        return 1;
    }

    int process_mul() {
        current_lexem = "binary operator";
        current_value = "*";
        return 1;
    }

    int process_div()  {
        current_lexem = "binary operator";
        current_value = "/";
        return 1;
    }

    int process_assign() {
        current_lexem = "binary operator";
        current_value = "=";
        return 1;
    }

    int process_eq()  {
        current_lexem = "comparing operator";
        current_value = "==";
        return 1;
    }

    int process_not_eq()  {
        current_lexem = "comparing operator";
        current_value = "!=";
        return 1;
    }

    int process_less()  {
        current_lexem = "comparing operator";
        current_value = "<";
        return 1;
    }

    int process_greater() {
        current_lexem = "comparing operator";
        current_value = ">";
        return 1;
    }

    int process_less_or_eq() {
        current_lexem = "comparing operator";
        current_value = "<=";
        return 1;
    }

    int process_greater_or_eq() {
        current_lexem = "comparing operator";
        current_value = ">=";
        return 1;
    }

    int process_left_paren() {
        current_lexem = "identifier";
        current_value = "(";
        return 1;
    }

    int process_right_paren() {
        current_lexem = "identifier";
        current_value = ")";
        return 1;
    }

    int process_left_brace() {
        current_lexem = "identifier";
        current_value = "{";
        return 1;
    }

    int process_right_brace() {
        current_lexem = "identifier";
        current_value = "}";
        return 1;
    }

    int process_semicolon() {
        current_lexem = "identifier";
        current_value = ";";
        return 1;
    }

    int process_id() {
        current_lexem = "variable";
        current_value = yytext;
        return 1;
    }

    int process_number() {
        current_lexem = "number";
        current_value = yytext;
        return 1;
    }

public:
  int yylex() override;
  void print_current() const {
    std::cout << current_lexem << " <" << current_value << ">" << std::endl;
  }
};

} // namespace language

#endif // FRONTEND_INCLUDE_LEXER_HPP
