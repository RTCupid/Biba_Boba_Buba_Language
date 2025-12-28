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
  #include <iostream>
  namespace language { class Lexer; }
  #include "node.hpp"
  #include "ast_factory.hpp"

  using language::AST_Factory;
  using language::Binary_operators;
  using language::Unary_operators;
  using language::nametable_t;
}

%code {
  #include "config.hpp"
  #include "lexer.hpp"
  #include "scope.hpp"
  #include <iostream>

  int yylex(yy::parser::semantic_type*   yylval,
            yy::parser::location_type*   yylloc,
            language::Lexer*             scanner)
  {
      int line_before = scanner->get_line();
      int column_before = scanner->get_column();

      auto tt = scanner->yylex();

      yylloc->begin.line = line_before;
      yylloc->begin.column = column_before;
      yylloc->end.line = scanner->get_line();
      yylloc->end.column = scanner->get_column() - 1;

      if (tt == yy::parser::token::TOK_NUMBER)
        yylval->build<int>() = std::stoi(scanner->YYText());

      if (tt == yy::parser::token::TOK_ID)
        yylval->build<std::string>() = scanner->YYText();

      return tt;
  }

  language::Scope scopes;
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
%token TOK_REM_DIV       "%"
%token TOK_AND           "&"
%token TOK_XOR           "^"
%token TOK_OR            "|"

/* --- Logical operators --- */
%token TOK_NOT           "!"

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

%type <language::StmtList>             stmt_list
%type <language::Statement_ptr>        statement
%type <language::Statement_ptr>        assignment_stmt if_stmt while_stmt print_stmt block_stmt empty_stmt
%type <language::Expression_ptr>       expression input bitwise_op equality relational add_sub mul_div unary primary assignment_expr

%start program

%%

program        : stmt_list TOK_EOF
                {
                  scopes.push(nametable_t{});
                  root = AST_Factory::makeProgram(std::move($1));
                  scopes.pop();
                }
               ;

stmt_list      :
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
               | if_stmt
                 { $$ = std::move($1); }
               | while_stmt
                 { $$ = std::move($1); }
               | print_stmt TOK_SEMICOLON
                 { $$ = std::move($1); }
               | block_stmt
                 { $$ = std::move($1); }
               | empty_stmt
                 { $$ = std::move($1); }
               ;

empty_stmt     : TOK_SEMICOLON
                {
                  $$ = AST_Factory::makeEmpty();
                }

block_stmt     : TOK_LEFT_BRACE stmt_list TOK_RIGHT_BRACE
                {
                  scopes.push(nametable_t{});
                  $$ = AST_Factory::makeBlock(std::move($2));
                  scopes.pop();
                }
               ;

assignment_stmt: TOK_ID TOK_ASSIGN expression
                {
                  auto variable = AST_Factory::makeVariable(std::move($1));
                  $$ = AST_Factory::makeAssignmentStmt(
                    std::move(variable),
                    std::move($3));

                  auto var_name = variable->get_name();

                  if (!scopes.find(var_name))
                    scopes.add_variable(var_name, true);
                }
                ;

if_stmt        : TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement %prec PREC_IFX
                {
                  $$ = AST_Factory::makeIf(std::move($3), std::move($5));
                }
               | TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement TOK_ELSE statement
                {
                  $$ = AST_Factory::makeIf(std::move($3), std::move($5), std::move($7));
                }
               ;

while_stmt     : TOK_WHILE TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement
                {
                  $$ = AST_Factory::makeWhile(std::move($3), std::move($5));
                }
               ;

print_stmt     : TOK_PRINT expression
                {
                  $$ = AST_Factory::makePrint(std::move($2));
                }
               ;

expression     : bitwise_op
                  { $$ = std::move($1); }
                | assignment_expr
                  { $$ = std::move($1); }
               ;

bitwise_op     : equality
                  { $$ = std::move($1); }
               | bitwise_op TOK_AND equality
                  { $$ = AST_Factory::makeBinaryOp(Binary_operators::And, std::move($1), std::move($3)); }
               | bitwise_op TOK_XOR equality
                  { $$ = AST_Factory::makeBinaryOp(Binary_operators::Xor, std::move($1), std::move($3)); }
               | bitwise_op TOK_OR  equality
                  { $$ = AST_Factory::makeBinaryOp(Binary_operators::Or, std::move($1), std::move($3)); }
               ;

equality       : relational
                 { $$ = std::move($1); }
               | equality TOK_EQ  relational
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Eq,  std::move($1), std::move($3)); }
               | equality TOK_NEQ relational
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Neq,  std::move($1), std::move($3)); }
               ;

relational     : add_sub
                 { $$ = std::move($1); }
               | relational TOK_LESS          add_sub
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Less, std::move($1), std::move($3)); }
               | relational TOK_LESS_OR_EQ    add_sub
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::LessEq, std::move($1), std::move($3)); }
               | relational TOK_GREATER       add_sub
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Greater, std::move($1), std::move($3)); }
               | relational TOK_GREATER_OR_EQ add_sub
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::GreaterEq, std::move($1), std::move($3)); }
               ;

add_sub        : mul_div
                 { $$ = std::move($1); }
               | add_sub TOK_PLUS  mul_div
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Add, std::move($1), std::move($3)); }
               | add_sub TOK_MINUS mul_div
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Sub, std::move($1), std::move($3)); }
               ;

mul_div        : unary
                 { $$ = std::move($1); }
               | mul_div TOK_MUL unary
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Mul, std::move($1), std::move($3)); }
               | mul_div TOK_DIV unary
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::Div, std::move($1), std::move($3)); }
               | mul_div TOK_REM_DIV unary
                 { $$ = AST_Factory::makeBinaryOp(Binary_operators::RemDiv, std::move($1), std::move($3)); }
               ;

unary          : TOK_MINUS unary
                { $$ = AST_Factory::makeUnaryOp(Unary_operators::Neg, std::move($2)); }
               | TOK_PLUS unary
                { $$ = AST_Factory::makeUnaryOp(Unary_operators::Neg, std::move($2)); }
               | TOK_NOT unary
                { $$ = AST_Factory::makeUnaryOp(Unary_operators::Not, std::move($2)); }
               | primary
                { $$ = std::move($1); }
               ;

primary        : TOK_NUMBER
                { $$ = AST_Factory::makeNumber($1); }
               | TOK_ID
                { $$ = AST_Factory::makeVariable(std::move($1)); }
               | TOK_LEFT_PAREN expression TOK_RIGHT_PAREN
                { $$ = std::move($2); }
               | input
                { $$ = std::move($1); }
               ;

input          : TOK_INPUT
                {
                  $$ = AST_Factory::makeInput();
                }
               ;

assignment_expr: TOK_ID TOK_ASSIGN expression
                {
                  $$ = AST_Factory::makeAssignmentExpr(
                    std::move(AST_Factory::makeVariable(std::move($1))),
                    std::move($3));
                }
               ;
%%

void yy::parser::error(const location& l, const std::string& m) {
    std::cerr << "Syntax error at line " << l.begin.line
              << ", column " << l.begin.column << ": " << m << '\n';
}
