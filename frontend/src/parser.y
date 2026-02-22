%language "c++"
%defines "parser.hpp"
%locations
%define parse.error detailed
%define api.value.type variant
%define api.parser.class {parser}

%nonassoc PREC_IFX
%nonassoc TOK_ELSE

%lex-param   { language::Lexer *scanner }
%parse-param { language::Lexer *scanner }
%parse-param { language::Node_pool &pool }
%parse-param { language::program_ptr &root }
%parse-param { language::My_parser *my_parser }

%code requires {
  #include <string>
  #include <iostream>
  #include "config.hpp"
  #include "node.hpp"
  #include "node_pool.hpp"
  #include "scope.hpp"

  namespace language { class Lexer; }
  namespace language { class My_parser; }

  using language::Node_pool;
  using language::Binary_operators;
  using language::Unary_operators;
  using language::nametable_t;
  using language::name_t;

  template<typename T>
  void push_scope(T* parser, nametable_t&& nametable);

  template<typename T>
  void pop_scope(T* parser);

  template<typename T>
  bool find_in_scopes(T* parser, const name_t& var_name);

  template<typename T>
  void add_var_to_scope(T* parser, const name_t& var_name);
}

%code {
  #include "config.hpp"
  #include "lexer.hpp"
  #include "error_collector.hpp"
  #include "my_parser.hpp"
  #include <iostream>
  #include <string>

  template<typename T>
  void push_scope(T* parser, nametable_t&& nametable) {
    parser->scopes.push(nametable);
  }

  template<typename T>
  void pop_scope(T* parser) {
    parser->scopes.pop();
  }

  template<typename T>
  bool find_in_scopes(T* parser, const name_t& var_name) {
    return parser->scopes.find(var_name);
  }

  template<typename T>
  void add_var_to_scope(T* parser, const name_t& var_name) {
    parser->scopes.add_variable(var_name);
  }

  int yylex(yy::parser::semantic_type* yylval,
            yy::parser::location_type* yylloc,
            language::Lexer*           scanner) {
    int line_before = scanner->get_line();

    auto tt = scanner->yylex();

    yylloc->begin.line = line_before;
    yylloc->begin.column = scanner->get_column() - scanner->get_yyleng();
    yylloc->end.line = scanner->get_line();
    yylloc->end.column = scanner->get_column();

    if (tt == yy::parser::token::TOK_NUMBER)
        yylval->build<int>() = std::stoi(scanner->YYText());

    if (tt == yy::parser::token::TOK_ID)
        yylval->build<std::string>() = scanner->YYText();

    return tt;
  }

  void yy::parser::error(const location& loc, const std::string& msg) {
    my_parser->error_collector.add_error(loc, msg, my_parser->get_line_content(loc.begin.line));
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
%token TOK_REM_DIV       "%"
%token TOK_AND           "&"
%token TOK_XOR           "^"
%token TOK_OR            "|"

/* --- Logical operators --- */
%token TOK_NOT           "!"
%token TOK_LOG_OR        "||"
%token TOK_LOG_AND       "&&"

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

%type <language::StmtList>             toplevel_stmt_list
%type <language::Statement_ptr>        toplevel_statement
%type <language::StmtList>             stmt_list
%type <language::Statement_ptr>        statement
%type <language::Statement_ptr>        assignment_stmt if_stmt while_stmt print_stmt block_stmt empty_stmt
%type <language::Expression_ptr>       expression bitwise_op equality relational add_sub mul_div unary primary assignment_expr or and


%start program

%%

program        : toplevel_stmt_list TOK_EOF
                {
                  root = my_parser->pool.make<language::Program>($1);
                }
               ;

toplevel_stmt_list: 
                {
                  $$ = language::StmtList{};
                }
               | toplevel_stmt_list toplevel_statement
                {
                  $1.push_back($2);
                  $$ = $1;
                }
               ;

toplevel_statement: statement
                {
                  $$ = $1;
                }
               | TOK_RIGHT_BRACE
                {
                  error(@1, "unmatched '}'");
                }
                ;

stmt_list: 
                {
                  $$ = language::StmtList{};
                }
               | stmt_list statement
                {
                  $1.push_back($2);
                  $$ = $1;
                }
               ;

statement      : assignment_stmt TOK_SEMICOLON
                 { $$ = $1; }
               | if_stmt
                 { $$ = $1; }
               | while_stmt
                 { $$ = $1; }
               | print_stmt TOK_SEMICOLON
                 { $$ = $1; }
               | block_stmt
                 { $$ = $1; }
               | empty_stmt
                 { $$ = $1; }
               | error TOK_SEMICOLON
                 {
                   yyerrok;
                 }
               ;

empty_stmt     : TOK_SEMICOLON
                {
                  $$ = my_parser->pool.make<language::Empty_stmt>();
                }

block_stmt     : TOK_LEFT_BRACE
                {
                  push_scope(my_parser, nametable_t{});
                }
                stmt_list
                TOK_RIGHT_BRACE
                {
                  pop_scope(my_parser);
                  $$ = my_parser->pool.make<language::Block_stmt>($3);
                }
               ;

assignment_stmt: TOK_ID TOK_ASSIGN expression
                {
                  auto variable = my_parser->pool.make<language::Variable>($1);
                  auto var_name = variable->get_name();

                  if (!find_in_scopes(my_parser, var_name))
                    add_var_to_scope(my_parser, var_name);

                  $$ = my_parser->pool.make<language::Assignment_stmt>(variable, $3);
                }
               ;

if_stmt        : TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement %prec PREC_IFX
                {
                  $$ = my_parser->pool.make<language::If_stmt>($3, $5);
                }
               | TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement TOK_ELSE statement
                {
                  $$ = my_parser->pool.make<language::If_stmt>($3, $5, $7);
                }
               | TOK_IF error TOK_RIGHT_PAREN statement %prec PREC_IFX
                {
                  yyerrok;
                }
               | TOK_IF TOK_LEFT_PAREN error statement %prec PREC_IFX
                {
                  yyerrok;
                }
               ;

while_stmt     : TOK_WHILE TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement
                {
                  $$ = my_parser->pool.make<language::While_stmt>($3, $5);
                }
               | TOK_WHILE error TOK_RIGHT_PAREN statement
                {
                  yyerrok;
                }
               | TOK_WHILE TOK_LEFT_PAREN error statement
                {
                  yyerrok;
                }
               ;

print_stmt     : TOK_PRINT expression
                {
                  $$ = my_parser->pool.make<language::Print_stmt>($2);
                }
               ;

expression     : assignment_expr
                {
                  $$ = $1;
                }
              ;

or            : and { $$ = $1; }
              | or TOK_LOG_OR and
                { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::LogOr, $1, $3); }
              ;

and           : bitwise_op { $$ = $1; }
                | and TOK_LOG_AND bitwise_op
                  { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::LogAnd, $1, $3); }
                ;

bitwise_op     : equality
                  { $$ = $1; }
               | bitwise_op TOK_AND equality
                  { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::And, $1, $3); }
               | bitwise_op TOK_XOR equality
                  { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Xor, $1, $3); }
               | bitwise_op TOK_OR  equality
                  { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Or, $1, $3); }
               ;

equality       : relational
                 { $$ = $1; }
               | equality TOK_EQ  relational
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Eq,  $1, $3); }
               | equality TOK_NEQ relational
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Neq,  $1, $3); }
               ;

relational     : add_sub
                 { $$ = $1; }
               | relational TOK_LESS          add_sub
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Less, $1, $3); }
               | relational TOK_LESS_OR_EQ    add_sub
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::LessEq, $1, $3); }
               | relational TOK_GREATER       add_sub
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Greater, $1, $3); }
               | relational TOK_GREATER_OR_EQ add_sub
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::GreaterEq, $1, $3); }
               ;

add_sub        : mul_div
                 { $$ = $1; }
               | add_sub TOK_PLUS  mul_div
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Add, $1, $3); }
               | add_sub TOK_MINUS mul_div
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Sub, $1, $3); }
               ;

mul_div        : unary
                 { $$ = $1; }
               | mul_div TOK_MUL unary
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Mul, $1, $3); }
               | mul_div TOK_DIV unary
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::Div, $1, $3); }
               | mul_div TOK_REM_DIV unary
                 { $$ = my_parser->pool.make<language::Binary_operator>(Binary_operators::RemDiv, $1, $3); }
               ;

unary          : TOK_MINUS unary
                { $$ = my_parser->pool.make<language::Unary_operator>(Unary_operators::Neg, $2); }
               | TOK_PLUS unary
                { $$ = my_parser->pool.make<language::Unary_operator>(Unary_operators::Plus, $2); }
               | TOK_NOT unary
                { $$ = my_parser->pool.make<language::Unary_operator>(Unary_operators::Not, $2); }
               | primary
                { $$ = $1; }
               ;

primary        : TOK_NUMBER
                { $$ = my_parser->pool.make<language::Number>($1); }
               | TOK_ID
                {
                  auto variable = my_parser->pool.make<language::Variable>($1);
                  if (!find_in_scopes(my_parser, variable->get_name())) {
                    error(@1, "\'" + variable->get_name() + "\' was not declared in this scope");
                  }

                  $$ = variable;
                }
               | TOK_LEFT_PAREN expression TOK_RIGHT_PAREN
                { $$ = $2; }
               | TOK_INPUT
                { $$ = my_parser->pool.make<language::Input>(); }
               ;

assignment_expr
              : or { $$ = $1; }
              | TOK_ID TOK_ASSIGN assignment_expr
                {
                  auto variable = my_parser->pool.make<language::Variable>($1);
                  auto var_name = variable->get_name();

                  if (!find_in_scopes(my_parser, var_name))
                    add_var_to_scope(my_parser, var_name);

                  $$ = my_parser->pool.make<language::Assignment_expr>(variable, $3);
                }
              ;
%%
