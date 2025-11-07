%language "c++"

%skeleton "lalr1.cc"
%defines
%define api.value.type variant
%param {yy::NumDriver* driver}

%code requires {
#include <algorithm>
#include <string>
#include <vector>

namespace language { class Driver; }
}

%code {
#include "driver.hpp"

namespace language {

parser::token_type yylex(parser::semantic_type* yylval,
                         NumDriver* driver);
} /* namespace language */

}

%token
    MINUS           "-"
    PLUS            "+"
    MUL             "*"
    DIV             "/"
    IF              "if"
    ELSE            "else"
    WHILE           "while"
    PRINT           "print"
    INPUT           "?"
    ASSIGN          "="
    EQ              "=="
    NOT_EQ          "!="
    LESS            "<"
    GREATER         ">"
    LESS_OR_EQ      "<="
    GREATER_OR_EQ   ">="
    LEFT_PAREN      "("
    RIGHT_PAREN     ")"
    LEFT_BRACE      "{"
    RIGHT_BRACE     "}"
    SEMICOLON       ";"
    ERR
;

 %token <int> NUMBER

%start program

%%

program: /* empty */ { }
;

%%

namespace language {

parser::token_type yylex(parser::semantic_type* yylval,
                         NumDriver* driver) {
    return driver->yylex(yylval);
}

void parser::error(const std::string&) {}

} /* namespace language */
