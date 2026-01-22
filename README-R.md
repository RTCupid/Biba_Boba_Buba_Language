<div align="center">

# Implementation of the "Biba-Boba-Buba language" programming language in C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

- This project is an implementation of the `ParaCL` programming language from the C++ course by K. I. Vladimirov.

## README in other languages

1. [Русский](/README-R.md)
2. [English](/README.md)

## Table of contents
Introduction:
- [Running the program](#running-the-program)
- [Introduction](#introduction)
- [Approach](#approach)

Language usage guide:
- [Language features](#language-features)

Реализация фронтенда:
- [Реализация лексического анализатора](#реализация-лексического-анализатора)
- [Реализация синтаксического анализатора](#реализация-синтаксического-анализатора)
- [Реализация сборщика ошибок](#реализация-сборщика-ошибок)
- [Реализация областей видимости](#реализация-областей-видимости)
- [Реализация симулятора](#реализация-симулятора)

Дополнительно:
- [Использование dump](#использование-dump)
- [Авторы проекта](#авторы-проекта)

### Running the program
Clone the repository, then build and compile it with the following commands:

```
git clone git@github.com:RTCupid/Biba_Boba_Buba_Language.git
cd Super_Biba_Boba_Language
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run the program in the following format:

```bash
./build/frontend/frontend <program file name>
```

## Introduction
Building your own programming language is a fundamental task in computer science. It helps you explore how computations work in practice. Creating a language with a C-like syntax makes it easier to understand compiler architecture. This process shows how high-level language constructs are translated into intermediate representations.

A manual implementation of a lexer and a parser comes with serious difficulties. This approach requires writing and debugging low-level code, which becomes especially painful when the grammar changes. Handling operator precedence and associativity is not trivial and makes language maintenance very time-consuming.

Using tools like `Flex` and `Bison` helps automate the creation of analyzers. `Flex` generates an efficient scanner from regular expressions, and `Bison` builds an LALR(1) parser that performs syntax analysis with a one-token lookahead. This approach speeds up development and makes it easier and safer to modify the grammar.

## Approach
An Extended Backus–Naur Form (`EBNF`) [1] is suitable for describing the grammar. To generate the lexer and the parser, you can use `Flex` and `Bison`.
To execute programs, you can implement an interpreter that walks through the `AST` using the `Visitor` abstraction and simulates program execution.

## Language features

A grammar for the target programming language was created. Below is its description in a format close to `EBNF` [1]:

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

В языке поддержаны области видимости переменных

## Реализация лексического анализатора
Реализована генерация лексического анализатора при помощи `Flex` (см. [lexer.l](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/lexer.l)).

Defined:

<details>
<summary>lexical constructs and processing rules</summary>

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

Lexeme processing functions are defined in the `Lexer` class, which inherits from
`yyFlexLexer` (see [lexer.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/lexer.hpp)).
They return the corresponding parser token generated by `Bison`, which is required for `Bison` and `Flex` to work together.

To print full error information, the following methods were added to the `Lexer` class:

<details>
<summary>methods for getting token location</summary>

```C++
int get_line() const { return yylineno; }

int get_column() const { return yycolumn; }

int get_yyleng() const { return yyleng; }
```

</details>

## Реализация синтаксического анализатора
Для синтаксического анализа, добавлен класс `My_parser` (см. [my_parser.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/my_parser.hpp)). Он наследуется от `yy::parser`, который генерируется при помощи `Bison` (см. [parser.y](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/parser.y)), и содержит следующие поля и методы:

<details>
<summary>класс My_parser</summary>
  
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

The function that connects the parser with the lexer:

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

For numbers and variables, the value is saved into `yylval`. In other cases, only the token type is returned.

</details>

During parsing, an `AST` (abstract syntax tree) is built.
By adding new parsing rules, the execution order hierarchy was implemented.

## Error collector implementation
The `Error_collector` (see [error_collector.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/error_collector.hpp)) is implemented to collect errors.

It stores a `std::vector` with information about each error:

<details>
<summary>Error_info struct</summary>

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

`My_parser` contains an `Error_collector` field, which makes it possible to add errors directly during parsing.

## Scopes implementation
To support local variables, the `Scope` class was added (see [scope.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/scope.hpp)). It stores a vector of name tables for each scope and provides methods to push new scopes and pop the most recently added scope, as well as to search for a variable by name in all scopes that are visible at the current point in the program:

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

Во время синтаксического анализа строится `AST` (abstract-syntax-tree). 
При помощи введения новых правил для синтаксического анализа реализована иерархия порядка исполнения.

## Реализация сборщика ошибок
Для сбора ошибок реализован сборщик `Error_collector` (см. [error_collector.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/error_collector.hpp)).

Внутри себя он хранит `std::vector` с информацией о каждой ошибке:

<details>
<summary>структура Error_info</summary>

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

А также содержит методы для добавления и вывода ошибок:

<details>
<summary>методы Error_collector</summary>

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

`My_parser` содержит поле `Error_collector`, что позволяет добавлять ошибки прямо во время синтаксического анализа.

## Реализация областей видимости
Для поддержки локальных переменных добавлен класс `Scope` (см. [scope.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/scope.hpp)), который хранит вектор из таблиц имён для каждой области видимости и имеет методы для добавления новых и удаления крайних добавленных областей видимости, а также для поиска переменной по имени во всех доступных в данной точке программы областях:

<details>
<summary>класс Scope</summary>

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

Экземпляр класса `Scope` хранится в классе `My_parser` и используется для проверки наличия переменной в области видимости в процессе синтаксического анализа.  

## Реализация симулятора
Чтобы симулировать выполнение программы, реализован класс `Simulator` (см. [simulator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/simulator.hpp)), наследующийся от абстрактного класса `ASTVisitor`:

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

In `Simulator`, the virtual methods of `ASTVisitor` are overridden. Also, a function for evaluating expressions is introduced, which uses a special `ExpressionEvaluator` class (see [expr_evaluator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/expr_evaluator.hpp)):

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

`ExpressionEvaluator` is specialized only for expression evaluation. It contains the `result_` field to store the result, and `simulator_` —
a reference to the simulator that called it, so it can access the name table.

## Использование dump
Для включения опции графического дампа дерева нужно выставить флаг -GRAPH_DUMP, который по умолчанию отключен
```bash
cmake -S . -B build -DGRAPH_DUMP=ON
```
Построенное дерево `AST` можно посмотреть в графическом представлении при помощи `graphviz`. Для генерации изображения можно ввести
```bash
dot dot dump/dump.gv -Tsvg -o dump/dump.svg
```

As a result, you will get the following tree representation:

<details>
<summary>example of a generated AST</summary>

<div align="center">
  <img src="img/graph_dump.svg" alt="Dump Banner" width="1200">
</div>

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
1. Extended Backus–Naur Form (EBNF) [Electronic resource]: article. - https://divancoder.ru/2017/06/ebnf/ (accessed May 21, 2025)