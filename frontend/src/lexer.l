%option yyclass="Lexer"
%option c++

%{

#include <iostream>
#include "lexer.hpp"

int yylineno;

using namespace language;

%}

DIGIT    [0-9]
DIGIT1   [1-9]
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
NUMBER   {DIGIT}+
WHITESPACE [ \t\r\v]+
NEWLINE  \n

%%

{WHITESPACE}  { /* skip blanks and tabs */ }
{NEWLINE}     { ++yylineno; }
"//".*        { /* skip comments */ }

"if"          return process_if();
"else"        return process_else();
"while"       return process_while();
"print"       return process_print();
"?"           return process_input();
"+"           return process_plus();
"-"           return process_minus();
"*"           return process_mul();
"/"           return process_div();
"="           return process_assign();
"=="          return process_eq();
"!="          return process_not_eq();
"<"           return process_less();
">"           return process_greater();
"<="          return process_less_or_eq();
">="          return process_greater_or_eq();
"("           return process_left_paren();
")"           return process_right_paren();
"{"           return process_left_brace();
"}"           return process_right_brace();
";"           return process_semicolon();
{ID}          return process_id();
{NUMBER}      return process_number();

.             { std::cerr << "Unknown token: " << yytext << " in string:" << yylineno << std::endl; }

%%

