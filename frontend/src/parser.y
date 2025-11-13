%language "c++"
%defines "parser.hpp"
%locations
%define parse.error detailed
%define api.value.type variant

%nonassoc PREC_IFX
%nonassoc TOK_ELSE

%lex-param   { language::Lexer* scanner }
%parse-param { language::Lexer* scanner }
%parse-param { std::unique_ptr<language::Program> &root }

%code requires {
  #include <string>
  namespace language { class Lexer; }
  #include "node.hpp" 
  #include "dsl.hpp"
}

%code {
  #include "lexer.hpp"
  #include <iostream>
  static int yylex(yy::parser::value_type*      /*yylval*/,
                   yy::parser::location_type*   /*yylloc*/,
                   language::Lexer*             scanner)
  {
      return scanner->yylex();
  }
}

/* ________________________Tokens________________________ */
/* --- Keywords --- */
%token TOK_IF            "if"
%token TOK_ELSE          "else"
%token TOK_WHILE         "while"
%token TOK_PRINT         "print"
%token TOK_INPUT         "?"

/* --- Arithmetic operators --- */
%token TOK_PLUS          "+"
%token TOK_MINUS         "-"
%token TOK_MUL           "*"
%token TOK_DIV           "/"

/* --- Assignment --- */
%token TOK_ASSIGN        "="

/* --- Comparison operators --- */
%token TOK_EQ            "=="
%token TOK_NEQ           "!="
%token TOK_LESS          "<"
%token TOK_GREATER       ">"
%token TOK_LESS_OR_EQ    "<="
%token TOK_GREATER_OR_EQ ">="

/* --- Parentheses and separators --- */
%token TOK_LEFT_PAREN    "("
%token TOK_RIGHT_PAREN   ")"
%token TOK_LEFT_BRACE    "{"
%token TOK_RIGHT_BRACE   "}"
%token TOK_SEMICOLON     ";"

/* --- Tokens with semantic values --- */
%token <std::string> TOK_ID     "identifier"
%token <int>         TOK_NUMBER "number"

/* --- End of file --- */
%token TOK_EOF 0
/* ______________________________________________________ */

%start program

%%

program        : stmt_list TOK_EOF
                {
                  root = std::make_unique<language::Program>(std::move($1));
                }
               ;

stmt_list      : /* empty */
                {
                  $$ = language::StmtList{};  
                }
               | stmt_list statement
                {
                  $1.push_back(std::move($2));
                  $$ = std::move($1);
                }
               ;

statement      : assignment_stmt TOK_SEMICOLON
                 { $$ = std::move($1); }
               | input_stmt TOK_SEMICOLON
                 { $$ = std::move($1); }
               | if_stmt
                 { $$ = std::move($1); }
               | while_stmt
                 { $$ = std::move($1); }
               | print_stmt TOK_SEMICOLON
                 { $$ = std::move($1); }
               | block_stmt
                 { $$ = std::move($1); }
               ;

block_stmt     : TOK_LEFT_BRACE stmt_list TOK_RIGHT_BRACE 
                {
                  $$ = std::make_unique<language::Block_stmt>(std::move($2));
                }
               ;

assignment_stmt: TOK_ID TOK_ASSIGN expression 
                {
                  $$ = std::make_unique<language::Assignment_stmt>(std::move($1), std::move($3));
                }
                ;

input_stmt     : TOK_ID TOK_ASSIGN TOK_INPUT
                {
                  $$ = std::make_unique<language::Input_stmt>(std::move($1));
                }
               ;

if_stmt        : TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement %prec PREC_IFX
                {
                  $$ = std::make_unique<language::If_stmt>(std::move($3), std::move($5));
                }
               | TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement TOK_ELSE statement
                {
                  $$ = std::make_unique<language::If_stmt>(std::move($3), std::move($5), std::move($7));
                }
               ;

while_stmt     : TOK_WHILE TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement 
                {
                  $$ = std::make_unique<language::While_stmt>(std::move($3), std::move($5));
                }
               ;

print_stmt     : TOK_PRINT expression 
                {
                  $$ = std::make_unique<language::Print_stmt>(std::move($2));
                }
               ;

expression     : equality ;

equality       : relational
               | equality TOK_EQ  relational
               | equality TOK_NEQ relational
               ;

relational     : add_sub
               | relational TOK_LESS          add_sub
               | relational TOK_LESS_OR_EQ    add_sub
               | relational TOK_GREATER       add_sub
               | relational TOK_GREATER_OR_EQ add_sub
               ;

add_sub        : mul_div
               | add_sub TOK_PLUS  mul_div
               | add_sub TOK_MINUS mul_div
               ;

mul_div        : unary
               | mul_div TOK_MUL unary
               | mul_div TOK_DIV unary
               ;

unary          : TOK_MINUS unary
               | primary
               ;

primary        : TOK_NUMBER
               | TOK_ID
               | TOK_LEFT_PAREN expression TOK_RIGHT_PAREN
               ;
%%

void yy::parser::error(const location& l, const std::string& m) {
    std::cerr << "Syntax error at line " << l.begin.line
              << ", column " << l.begin.column << ": " << m << "\n";
}
