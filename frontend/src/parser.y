%language "c++"
%defines "parser.hpp"
%locations
%define parse.error detailed
%define api.value.type variant
%define api.parser.class {parser}

%nonassoc PREC_IFX
%nonassoc TOK_ELSE

%lex-param   { language::Lexer* scanner }
%parse-param { language::Lexer* scanner }
%parse-param { language::program_ptr &root }
%parse-param { language::My_parser* my_parser }

%code requires {
  #include <string>
  #include <iostream>
  #include "config.hpp"
  #include "node.hpp"
  #include "ast_factory.hpp"
  #include "iterative_ast_deleter.hpp"
  #include "scope.hpp"

  namespace language { class Lexer; }
  namespace language { class My_parser; }

  using language::AST_Factory;
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

%type <language::StmtList>             stmt_list
%type <language::Statement_ptr>        statement
%type <language::Statement_ptr>        assignment_stmt if_stmt while_stmt print_stmt block_stmt empty_stmt
%type <language::Expression_ptr>       expression bitwise_op equality relational add_sub mul_div unary primary assignment_expr or and


%start program

%%

program        : stmt_list TOK_EOF
                {
                  root = AST_Factory::makeProgram(std::move($1));
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
                  $$ = AST_Factory::make<language::Empty_stmt>();
                }

block_stmt     : TOK_LEFT_BRACE
                {
                  push_scope(my_parser, nametable_t{});
                }
                stmt_list
                TOK_RIGHT_BRACE
                {
                  pop_scope(my_parser);
                  $$ = AST_Factory::make<language::Block_stmt>(std::move($3));
                }
               ;

assignment_stmt: TOK_ID TOK_ASSIGN expression
                {
                  auto variable = AST_Factory::make<language::Variable>(std::move($1));
                  auto var_name = variable->get_name();

                  if (!find_in_scopes(my_parser, var_name))
                    add_var_to_scope(my_parser, var_name);

                  $$ = AST_Factory::make<language::Assignment_stmt>(
                    std::move(variable),
                    std::move($3));
                }
               ;

if_stmt        : TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement %prec PREC_IFX
                {
                  $$ = AST_Factory::make<language::If_stmt>(std::move($3), std::move($5));
                }
               | TOK_IF TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement TOK_ELSE statement
                {
                  $$ = AST_Factory::make<language::If_stmt>(std::move($3), std::move($5), std::move($7));
                }
               ;

while_stmt     : TOK_WHILE TOK_LEFT_PAREN expression TOK_RIGHT_PAREN statement
                {
                  $$ = AST_Factory::make<language::While_stmt>(std::move($3), std::move($5));
                }
               ;

print_stmt     : TOK_PRINT expression
                {
                  $$ = AST_Factory::make<language::Print_stmt>(std::move($2));
                }
               ;

expression     : assignment_expr
                {
                  $$ = std::move($1);
                }
              ;

or            : and { $$ = std::move($1); }
              | or TOK_LOG_OR and
                { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::LogOr, std::move($1), std::move($3)); }
              ;

and           : bitwise_op { $$ = std::move($1); }
                | and TOK_LOG_AND bitwise_op
                  { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::LogAnd, std::move($1), std::move($3)); }
                ;

bitwise_op     : equality
                  { $$ = std::move($1); }
               | bitwise_op TOK_AND equality
                  { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::And, std::move($1), std::move($3)); }
               | bitwise_op TOK_XOR equality
                  { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Xor, std::move($1), std::move($3)); }
               | bitwise_op TOK_OR  equality
                  { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Or, std::move($1), std::move($3)); }
               ;

equality       : relational
                 { $$ = std::move($1); }
               | equality TOK_EQ  relational
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Eq,  std::move($1), std::move($3)); }
               | equality TOK_NEQ relational
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Neq,  std::move($1), std::move($3)); }
               ;

relational     : add_sub
                 { $$ = std::move($1); }
               | relational TOK_LESS          add_sub
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Less, std::move($1), std::move($3)); }
               | relational TOK_LESS_OR_EQ    add_sub
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::LessEq, std::move($1), std::move($3)); }
               | relational TOK_GREATER       add_sub
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Greater, std::move($1), std::move($3)); }
               | relational TOK_GREATER_OR_EQ add_sub
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::GreaterEq, std::move($1), std::move($3)); }
               ;

add_sub        : mul_div
                 { $$ = std::move($1); }
               | add_sub TOK_PLUS  mul_div
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Add, std::move($1), std::move($3)); }
               | add_sub TOK_MINUS mul_div
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Sub, std::move($1), std::move($3)); }
               ;

mul_div        : unary
                 { $$ = std::move($1); }
               | mul_div TOK_MUL unary
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Mul, std::move($1), std::move($3)); }
               | mul_div TOK_DIV unary
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::Div, std::move($1), std::move($3)); }
               | mul_div TOK_REM_DIV unary
                 { $$ = AST_Factory::make<language::Binary_operator>(Binary_operators::RemDiv, std::move($1), std::move($3)); }
               ;

unary          : TOK_MINUS unary
                { $$ = AST_Factory::make<language::Unary_operator>(Unary_operators::Neg, std::move($2)); }
               | TOK_PLUS unary
                { $$ = AST_Factory::make<language::Unary_operator>(Unary_operators::Plus, std::move($2)); }
               | TOK_NOT unary
                { $$ = AST_Factory::make<language::Unary_operator>(Unary_operators::Not, std::move($2)); }
               | primary
                { $$ = std::move($1); }
               ;

primary        : TOK_NUMBER
                { $$ = AST_Factory::make<language::Number>($1); }
               | TOK_ID
                {
                  auto variable = AST_Factory::make<language::Variable>(std::move($1));
                  if (!find_in_scopes(my_parser, variable->get_name())) {
                    error(@1, "\'" + variable->get_name() + "\' was not declared in this scope");
                  }

                  $$ = std::move(variable);
                }
               | TOK_LEFT_PAREN expression TOK_RIGHT_PAREN
                { $$ = std::move($2); }
               | TOK_INPUT
                { $$ = AST_Factory::make<language::Input>(); }
               ;

assignment_expr
              : or { $$ = std::move($1); }
              | TOK_ID TOK_ASSIGN assignment_expr
                {
                  auto variable = AST_Factory::make<language::Variable>(std::move($1));
                  auto var_name = variable->get_name();

                  if (!find_in_scopes(my_parser, var_name))
                    add_var_to_scope(my_parser, var_name);

                  $$ = AST_Factory::make<language::Assignment_expr>(std::move(variable), std::move($3));
                }
              ;
%%
