%option yyclass="Lexer"
%option c++
%option nounput noinput

%{
  #include <string>
  #include <iostream>
  // #include "parser.hpp"
  #include "lexer.hpp"

  int yylineno;

  using namespace language;
%}

WHITESPACE    [ \t\r\v]+
ID            [a-zA-Z_][a-zA-Z0-9_]*
NUMBER        [0-9]+
LINE_COMMENT  "//".*
BLOCK_COMMENT "/*"([^*]|\*+[^*/])*\*+"/"
NEWLINE  \n

%%

{WHITESPACE}    { /* skip blanks and tabs */ }
{NEWLINE}       { ++yylineno; }

{LINE_COMMENT}  { /* skip */ }
{BLOCK_COMMENT} { /* skip */ }

"if"            { return process_if();   }
"else"          { return process_else(); }
"while"         { return process_while(); }
"print"         { return process_print(); }
"?"             { return process_input(); }

"=="            { return process_eq(); }
"!="            { return process_not_eq(); }
"<="            { return process_less_or_eq(); }
">="            { return process_greater_or_eq(); }
"="             { return process_assign(); }

"+"             { return process_plus(); }
"-"             { return process_minus(); }
"*"             { return process_mul(); }
"/"             { return process_div(); }

"<"             { return process_less(); }
">"             { return process_greater(); }

"("             { return process_left_paren(); }
")"             { return process_right_paren(); }
"{"             { return process_left_brace(); }
"}"             { return process_right_brace(); }
";"             { return process_semicolon(); }

{NUMBER}        { return process_number();}

{ID}            { return process_id(); }

.               {
                    std::cerr << "Unknown token: '" << yytext << "' at line " << yylineno << std::endl;;
                    return -1;
                }

<<EOF>>         { return 0; }

%%

