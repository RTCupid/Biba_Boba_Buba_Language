<div align="center">

# Implementation of the “Biba-Boba-Buba language” programming language in C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

- This project is an implementation of the `ParaCL` programming language from K. I. Vladimirov’s C++ course.

## README in other languages

1. [Russian](/README-R.md)
2. [English](/README.md)

## Table of Contents
Introduction:
- [Running the program](#running-the-program)
- [Introduction](#introduction)
- [Methodology](#methodology)

Language usage guide:
- [Language features overview](#language-features-overview)

Frontend implementation:
- [Lexical analyzer implementation](#lexical-analyzer-implementation)
- [Syntax analyzer implementation](#syntax-analyzer-implementation)
- [Error collector implementation](#error-collector-implementation)
- [Scopes implementation](#scopes-implementation)
- [Simulator implementation](#simulator-implementation)

Additional:
- [Using dump](#using-dump)
- [Project structure](#project-structure)
- [Project authors](#project-authors)

### Running the program
Cloning the repository, building, and compiling can be done with the following commands:

```
git clone https://github.com/RTCupid/Biba_Boba_Buba_Language.git
cd Biba_Boba_Buba_Language
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run the program in the following format:
```
./build/frontend/frontend <program file name>
```

## Introduction
Developing your own programming language is a fundamental task in computer science that allows you to explore the principles of computation in practice. Creating a language with a C-like syntax helps to better understand compiler architecture. This process reveals the internal logic of translating high-level constructs into intermediate representations.

Manually implementing lexical and syntax analyzers comes with significant challenges. This approach requires writing and debugging low-level code, which is especially problematic when the grammar changes. Handling operator precedence and associativity becomes a non-trivial task, making language maintenance extremely labor-intensive.

Using tools like `Flex` and `Bison` helps automate analyzer generation. `Flex` generates an efficient scanner based on regular expressions, while `Bison` builds an LALR(1) parser that performs syntax analysis with a one-token lookahead. This approach significantly speeds up development while improving reliability and making grammar changes easier.

## Methodology
The grammar can be described using the `EBNF` format [1]. `Flex` and `Bison` can be used to generate the lexical and syntax analyzers.
To execute the program, you can implement an interpreter that traverses the `AST` using the `Visitor` abstraction and simulates program execution.

## Language features overview

A grammar for the target programming language has been created. Below is its description in a format close to `EBNF` [1]:

<details>
<summary>Grammar</summary>

```
Program        ::= StmtList EOF

StmtList       ::= /* empty */ |  StmtList Statement 

Statement      ::= AssignmentStmt ';' | InputStmt ';' | IfStmt | WhileStmt | PrintStmt ';' | BlockStmt | ';'

BlockStmt      ::= '{' StmtList '}'
AssignmentStmt ::= Var '=' Expression
InputStmt      ::= Var '=' '?'
IfStmt         ::= 'if'    '(' Expression ')' Statement [ 'else' Statement ]
WhileStmt      ::= 'while' '(' Expression ')' Statement
PrintStmt      ::= 'print' Expression

Expression     ::= AssignmentExpr
AssignmentExpr ::= Or | Var '=' AssignmentExpr
Or             ::= And | Or '||' And
And            ::= BitwiseOp | And '&&' BitwiseOp
BitwiseOp      ::= Equality | BitwiseOp '&' Equality | BitwiseOp '^' Equality | BitwiseOp '|'  Equality
Equality       ::= Relational ( ( '==' | '!=' ) Relational )*
Relational     ::= AddSub ( ( '<' | '>' | '<=' | '>=' ) AddSub )*
AddSub         ::= MulDiv ( ( '+' | '-' ) MulDiv )*
MulDiv         ::= Unary  ( ( '*' | '/' ) Unary )*
Unary          ::= '-' Unary | '+' Unary | '~' Unary | Primary
Primary        ::= '(' Expression ')' | Var | Number

Var            ::= [A-Za-z_][A-Za-z0-9_]*
Number         ::= [1-9][0-9]* | '0'
EOF            ::= __end_of_file__
```

</details>

The language supports variable scopes.

## Lexical analyzer implementation
Lexical analyzer generation is implemented using `Flex` (see [lexer.l](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/lexer.l)).

Defined:

<details>
<summary>lexical constructs and rules for processing them</summary>

```l
WHITESPACE    [ \t\r\v]+
ID            [a-zA-Z_][a-zA-Z0-9_]*
NUMBER        [0-9]+
NUMBER1       [1-9]+
ZERO          0
LINE_COMMENT  "//".*
BLOCK_COMMENT "/*"([^*]|\*+[^*/])*\*+"/"
NEWLINE  \n

%%

{WHITESPACE}    { yycolumn += yyleng; }
{NEWLINE}       { ++yylineno; yycolumn = 1; }

{LINE_COMMENT}  { yycolumn += yyleng; }
{BLOCK_COMMENT} { /* skip */ }

"if"            { yycolumn += yyleng; return process_if();   }
"else"          { yycolumn += yyleng; return process_else(); }
"while"         { yycolumn += yyleng; return process_while(); }
"print"         { yycolumn += yyleng; return process_print(); }
"?"             { yycolumn += yyleng; return process_input(); }

"||"             { yycolumn += yyleng; return process_log_or(); }
"&&"             { yycolumn += yyleng; return process_log_and(); }

"!"             { yycolumn += yyleng; return process_not(); }
"=="            { yycolumn += yyleng; return process_eq(); }
"!="            { yycolumn += yyleng; return process_not_eq(); }
"<="            { yycolumn += yyleng; return process_less_or_eq(); }
">="            { yycolumn += yyleng; return process_greater_or_eq(); }

"="             { yycolumn += yyleng; return process_assign(); }

"+"             { yycolumn += yyleng; return process_plus(); }
"-"             { yycolumn += yyleng; return process_minus(); }
"*"             { yycolumn += yyleng; return process_mul(); }
"/"             { yycolumn += yyleng; return process_div(); }
"%"             { yycolumn += yyleng; return process_rem_div(); }
"&"             { yycolumn += yyleng; return process_and(); }
"^"             { yycolumn += yyleng; return process_xor(); }
"|"             { yycolumn += yyleng; return process_or(); }

"<"             { yycolumn += yyleng; return process_less(); }
">"             { yycolumn += yyleng; return process_greater(); }

"("             { yycolumn += yyleng; return process_left_paren(); }
")"             { yycolumn += yyleng; return process_right_paren(); }
"{"             { yycolumn += yyleng; return process_left_brace(); }
"}"             { yycolumn += yyleng; return process_right_brace(); }
";"             { yycolumn += yyleng; return process_semicolon(); }

{NUMBER1}{NUMBER}* { yycolumn += yyleng; return process_number(); }
{ZERO}          { yycolumn += yyleng; return process_number(); }

{ID}            { yycolumn += yyleng; return process_id(); }

.               {
                    std::cerr << "Unknown token: '" << yytext << "' at line " << yylineno << std::endl;;
                    return -1;
                }

<<EOF>>         { return 0; }

%%
```

</details>

Functions for processing lexemes are defined in the `Lexer` class, which inherits from
`yyFlexLexer` (see [lexer.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/lexer.hpp)).
They return the corresponding parser token generated by `Bison`. This is done to make `Bison` and `Flex` work together.

To output full error information, the following were added to the `Lexer` class:

<details>
<summary>functions to obtain token location</summary>

```C++
int get_line() const { return yylineno; }

int get_column() const { return yycolumn; }

int get_yyleng() const { return yyleng; }
```

</details>

## Syntax analyzer implementation
For syntax analysis, the `My_parser` class was added (see [my_parser.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/my_parser.hpp)). It inherits from `yy::parser`, which is generated using `Bison` (see [parser.y](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/parser.y)), and contains the following fields and methods:

<details>
<summary>My_parser class</summary>

```C++
class My_parser final : public yy::parser {
  private:
    Lexer *scanner_;
    std::unique_ptr<Program> root_;
    std::vector<std::string> source_lines_;

  public:
    Error_collector error_collector;
    Scope scopes;

    My_parser(Lexer *scanner, std::unique_ptr<language::Program> &root,
              const std::string &program_file)
        : yy::parser(scanner, root, this), scanner_(scanner),
          root_(std::move(root)), error_collector(program_file) {
        read_source(program_file);
    }
    ...
};
```

</details>

The function through which the parser interacts with the lexer:

<details>
<summary>yylex function</summary>

```C++
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
```

For numbers and variables, the value is stored in `yylval`; in other cases, the token type is returned.

</details>

During syntax analysis, an `AST` (abstract syntax tree) is built.
By introducing new syntax rules, an execution-order hierarchy is also implemented.

## Error collector implementation
An `Error_collector` was implemented to collect errors (see [error_collector.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/error_collector.hpp)).

Internally, it stores a `std::vector` with information about each error:

<details>
<summary>Error_info structure</summary>

```C++  
struct Error_info {
  const std::string program_file_;
  const yy::location loc_;
  const std::string msg_;
  const std::string line_with_error_;

  Error_info(const std::string program_file, const yy::location &loc,
             const std::string &msg, const std::string &line_with_error)
      : program_file_(program_file), loc_(loc), msg_(msg),
        line_with_error_(line_with_error) {}

  Error_info(const std::string program_file, const yy::location &loc,
             const std::string &msg)
      : program_file_(program_file), loc_(loc), msg_(msg) {}

  void print(std::ostream &os) const {
      ...
  }
};
```

</details>

It also contains methods for adding and printing errors:

<details>
<summary>Error_collector methods</summary>

```C++
void add_error(const yy::location &loc, const std::string &msg,
               const std::string &line_with_error) {
    errors_.push_back(Error_info{program_file_, loc, msg, line_with_error});
}

void add_error(const yy::location &loc, const std::string &msg) {
    errors_.push_back(Error_info{program_file_, loc, msg});
}

bool has_errors() const { return !errors_.empty(); }

void print_errors(std::ostream &os) const {
    if (!errors_.empty())
        for (auto &error : errors_)
            error.print(os);
}
```

</details>

`My_parser` contains an `Error_collector` field, which allows adding errors directly during syntax analysis.

## Scopes implementation
To support local variables, the `Scope` class was added (see [scope.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/scope.hpp)). It stores a vector of name tables for each scope and provides methods for adding new scopes, removing the last added scope, and searching for a variable by name across all scopes available at a given point in the program:

<details>
<summary>Scope class</summary>

```C++
class Scope final {
  private:
    std::vector<nametable_t> scopes_;

  public:
    Scope() {
        push(nametable_t{}); // add global scope
    }

    void push(nametable_t nametable) { scopes_.push_back(nametable); }

    void pop() { scopes_.pop_back(); }

    void add_variable(name_t &var_name, bool defined) {
        assert(!scopes_.empty());
        scopes_.back().emplace(var_name, defined);
    }

    bool find(name_t &var_name) const {
        for (auto it = scopes_.rbegin(), last_it = scopes_.rend();
             it != last_it; ++it) {
            auto var_iter = it->find(var_name);
            if (var_iter != it->end())
                return true;
        }

        return false;
    }
};
```

</details>

An instance of `Scope` is stored in `My_parser` and is used to check whether a variable exists in the current scope during syntax analysis.

## Simulator implementation
To simulate program execution, the `Simulator` class was implemented (see [simulator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/simulator.hpp)), inheriting from the abstract `ASTVisitor` class:

<details>
<summary>ASTVisitor class</summary>

```C++
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    virtual void visit(Program &node) = 0;
    virtual void visit(Block_stmt &node) = 0;
    virtual void visit(Empty_stmt &node) = 0;
    virtual void visit(Assignment_stmt &node) = 0;
    virtual void visit(Assignment_expr &node) = 0;
    virtual void visit(Input &node) = 0;
    virtual void visit(If_stmt &node) = 0;
    virtual void visit(While_stmt &node) = 0;
    virtual void visit(Print_stmt &node) = 0;
    virtual void visit(Binary_operator &node) = 0;
    virtual void visit(Unary_operator &node) = 0;
    virtual void visit(Number &node) = 0;
    virtual void visit(Variable &node) = 0;
};
```

</details>

In `Simulator`, the virtual functions of `ASTVisitor` are overridden, and a function for evaluating expressions is introduced. It uses a dedicated `ExpressionEvaluator` class (see [expr_evaluator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/expr_evaluator.hpp)):

<details>
<summary>evaluate_expression function</summary>

```C++
number_t Simulator::evaluate_expression(Expression &expression) {
    ExpressionEvaluator evaluator(*this);
    expression.accept(evaluator);
    return evaluator.get_result();
}
```

</details>

`ExpressionEvaluator` is specialized only for evaluating expressions; it contains the `result_` field to store the expression result,
as well as `simulator_` — a reference to the simulator from which it was called, so it has access to the name table.

## Using dump
To enable graphical AST dump, set the `-GRAPH_DUMP` flag, which is disabled by default:
```bash
cmake -S . -B build -DGRAPH_DUMP=ON
```
The constructed `AST` tree can be viewed graphically using `graphviz`. To generate an image, run:
```bash
dot dot dump/dump.gv -Tsvg -o dump/dump.svg
```
You will get the following tree representation:

<details>
<summary>example of a generated AST</summary>

<div align="center">
  <img src="img/graph_dump.svg" alt="Dump Banner" width="1200">
</div>

</details>

## Project structure

<details>
<summary>Project structure</summary>

```
├── build
├── CMakeLists.txt
├── contribution_guidelines.md
├── frontend
│   ├── CMakeLists.txt
│   ├── include
│   │   ├── ast_factory.hpp
│   │   ├── config.hpp
│   │   ├── driver.hpp
│   │   ├── dump_path_gen.hpp
│   │   ├── error_collector.hpp
│   │   ├── expr_evaluator.hpp
│   │   ├── lexer.hpp
│   │   ├── my_parser.hpp
│   │   ├── node.hpp
│   │   ├── scope.hpp
│   │   └── simulator.hpp
│   ├── src
│   │   ├── driver.cpp
│   │   ├── expr_evaluator.cpp
│   │   ├── graph_dump.cpp
│   │   ├── lexer.l
│   │   ├── main.cpp
│   │   ├── parser.y
│   │   └── simulator.cpp
│   └── tests
│       ├── CMakeLists.txt
│       ├── end_to_end
│           └── ...
│       └── unit
│           └── ...
├── img
│   └── ...
├── LICENSE
├── README.md
└── README-R.md
```

</details>

## Project authors

<div align="center">

  <a href="https://github.com/RTCupid">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/A.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <a href="https://github.com/BulgakovDmitry">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/D.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <a href="https://github.com/lavrt">
    <img src="https://raw.githubusercontent.com/RTCupid/Biba_Boba_Buba_Language/main/img/lesha.png" width="160" height="160" style="border-radius: 50%;">
  </a>
  <br>
  <a href="https://github.com/RTCupid"><strong>@RTCupid, </strong></a>
  <a href="https://github.com/BulgakovDmitry"><strong>@BulgakovDmitry, </strong></a>
  <a href="https://github.com/lavrt"><strong>@lavrt</strong></a>
  <br>
</div>

## 📚 References
1. Extended Backus–Naur form (EBNF) [Online resource]: article — https://divancoder.ru/2017/06/ebnf/ (accessed May 21, 2025)
