<div align="center">

# Реализация языка программирования Super Biba-Boba language на C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

- Данный проект представляет реализацию языка программирования `ParaCL` из курса C++ от К.И. Владимирова.

## README на других языках 

1. [Русский](/README-R.md)
2. [English](/README.md)

## Оглавление
Вступление:
- [Запуск программы](#запуск-программы)
- [Введение](#введение)
- [Методика](#методика)

Инструкция по использованию языка:
- [Описание возможностей языка](#описание-возможностей-языка)

Реализация фронтенда:
- [Реализация лексического анализатора](#реализация-лексического-анализатора)
- [Реализация синтаксического анализатора](#реализация-синтаксического-анализатора)
- [Реализация симулятора](#реализация-симулятора)

Дополнительно:
- [Использование dump](#dump)
- [Авторы проекта](#авторы-проекта)

### Запуск программы
Клонирование репозитория, сборка и компиляция выполняется при помощи следующих команд:

```
git clone https://github.com/RTCupid/Super_Biba_Boba_Language.git
cd Super_Biba_Boba_Language
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Запуск программы производится в следующем формате:
```
./build/frontend/frontend <имя файла с программой>
```

## Введение
Разработка собственного языка программирования представляет собой фундаментальную задачу в компьютерных науках, позволяющую на практике исследовать принципы вычислений. Создание языка с C-подобным синтаксисом позволяет лучше понять архитектуру компиляторов. Этот процесс раскрывает внутреннюю логику трансляции высокоуровневых конструкций в промежуточные представления.

Ручная реализация лексического и синтаксического анализаторов сопряжена с существенными сложностями. Такой подход требует написания и отладки низкоуровневого кода, что особенно проблематично при модификации грамматики. Обработка приоритета операторов и ассоциативности становится нетривиальной задачей, делая поддержку языка чрезвычайно трудоёмкой.

Использование инструментов `Flex` и `Bison` позволяет автоматизировать создание анализаторов. `Flex` генерирует эффективный сканер на основе регулярных выражений, а `Bison` строит LALR(1)-парсер, выполняющий синтаксический анализ с опережающим просмотром в один токен. Этот подход значительно ускоряет разработку, обеспечивая надёжность и лёгкость модификации грамматики.

## Методика
Для описания грамматики подойдёт формат `РБНФ` [1]. Для генерации лексического и синтаксического анализаторов можно использовать `Flex` и `Bison`.
Для выполнения программы можно написать интерпретатор, который при помощи абстракции `Visitor'a` пройдёт по `AST` и просимулирует выполнение программы. 

## Описание возможностей языка

Составлена грамматика целевого языка программирования. Ниже приведено её описание в формате, близком к `РБНФ` [1]:

<details>
<summary>Грамматика</summary>
  
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

## Реализация лексического анализатора
Реализована генерация лексического анализатора при помощи `Flex` (см. [lexer.l](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/lexer.l)).

Определены:

<details>
<summary>лексические конструкции</summary>

```l
WHITESPACE    [ \t\r\v]+
ID            [a-zA-Z_][a-zA-Z0-9_]*
NUMBER        [0-9]+
NUMBER1       [1-9]+
ZERO          0
LINE_COMMENT  "//".*
BLOCK_COMMENT "/*"([^*]|\*+[^*/])*\*+"/"
NEWLINE  \n
```

</details>

и 

<details>
<summary>правила для их обработки</summary>
  
```y
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
```

</details>

Функции для обработки правил определены в классе `Lexer`, который наследуется от
`yyFlexLexer`(см. [lexer.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/lexer.hpp)).
Они возвращают соответствующий token парсера, который генерирует `Bison`, это сделано для совместной работы `Bison` и `Flex`.

Для вывода полной информации об ошибке в класс `Lexer` добавлены: 

<details>
<summary>функции для получения локации токена</summary>

```C++
int get_line() const { return yylineno; }

int get_column() const { return yycolumn; }

int get_yyleng() const { return yyleng; }
```

</details>

## Реализация синтаксического анализатора
Класс синтаксического анализатора наследуется от yy::parser, который генерируется при помощи Bison (см. [parser.y](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/parser.y)), и содержит следующие поля и методы:

<details>
<summary>класс Parser</summary>
  
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

Функция, через которую осуществляется взаимодействие парсера с лексером:

<details>
<summary>функция yylex</summary>

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

Для чисел и переменных сохраняется значение в `yylval`, в остальных случаях возвращается тип токена.

</details>

Во время синтаксического анализа строится `AST` (abstract-syntax-tree). 
При помощи введения новых правил для синтаксического анализа реализована иерархия порядка исполнения.

## Реализация симулятора
Чтобы симулировать выполнение программы, реализован класс `Simulator` (см. [simulator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/simulator.hpp)), наследующийся от абстрактного класса ASTVisitor:

<details>
<summary>класс ASTVisitor</summary>
  
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

В классе `Simulator` выполняется переопределение виртуальных функций `ASTVisitor`, а также вводится функция для вычисления выражений, которая использует специальный класс `ExpressionEvaluator` (см. [expr_evaluator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/expr_evaluator.hpp)):

<details>
<summary>функция evaluate_expression</summary>

```C++
number_t Simulator::evaluate_expression(Expression &expression) {
    ExpressionEvaluator evaluator(*this);
    expression.accept(evaluator);
    return evaluator.get_result();
}
```

</details>

`ExpressionEvaluator` специализируется только на вычислении выражений, содержит поле `result_` для сохранения результата выражения, а также `simulator_` - 
ссылку на симулятор, из которого он был вызван, чтобы иметь доступ к таблице имён.

## Dump
Построенное дерево AST можно посмотреть в графическом представлении при помощи graphviz. Для генерации изображения можно ввести
```bash
dot graph_dump/graph_dump.gv -Tsvg -o graph_dump/graph_dump.svg
```
Получится следующее представление дерева

<details>
<summary>пример сгенерированного AST</summary>
  
<div align="center">
  <img src="img/graph_dump.svg" alt="Dump Banner" width="1200">
</div>

</details>

## Авторы проекта

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

## 📚 Литература
1. Расширенная форма Бэккуса-Науэра [Электронный ресурс]: статья. -  https://divancoder.ru/2017/06/ebnf/ (дата обращения 21 мая 2025)
