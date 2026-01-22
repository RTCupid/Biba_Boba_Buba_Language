<div align="center">

# Implementation of the Biba-Boba-Buba Programming Language in C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

- This project is an implementation of the `ParaCL` programming language from the C++ course by K.I. Vladimirov.

- Check the [Contribution Guidelines](contribution_guidelines.md)

### Running the program
Cloning the repository, configuring and building are done with the following commands:

```bash
git clone https://github.com/RTCupid/Super_Biba_Boba_Language.git
cd Super_Biba_Boba_Language
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Run the program as follows:
```bash
./build/frontend/frontend <program file name>
```

## 📖 Introduction
Designing your own programming language is a fundamental task in computer science that enables hands-on exploration of computation principles. Creating a language with C-like syntax helps to better understand compiler architecture. This process reveals the internal logic of translating high-level constructs into intermediate representations.

Manually implementing lexical and syntax analyzers is associated with substantial complexity. This approach requires writing and debugging low-level code, which becomes especially problematic when the grammar evolves. Handling operator precedence and associativity is a nontrivial task, making language maintenance extremely labor-intensive.

Using tools such as `Flex` and `Bison` automates the construction of analyzers. `Flex` generates an efficient scanner based on regular expressions, while `Bison` builds an LALR(1) parser that performs syntax analysis with one-token lookahead. This approach significantly speeds up development, providing reliability and ease of grammar modifications.

## 🔑 Methodology
Implementing the frontend of a programming language includes several tasks:
1) defining the grammar;
2) splitting a program into tokens, lexical analysis;
3) building the `AST`, syntax analysis.

The `EBNF` format [1] is suitable for describing the grammar. To generate lexical and syntax analyzers, one can use `Flex` and `Bison`.
To execute a program, you can write an interpreter that, using the `Visitor` abstraction, traverses the `AST` and simulates the program execution. 

## 👨‍💻 Workflow
### Grammar description

The grammar of the target programming language has been designed. Below is its description in a format close to `EBNF` [1]:

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

### Lexical analyzer implementation
At the first stage, classes for tokens, AST nodes, the lexer and the parser were implemented:
<details>
<summary>Show code block</summary>
  
```C++
class Parser {
    std::size_t pc_ = 0;
    std::unique_ptr<Node> root_{nullptr};
  public:
    bool parse(const std::vector<std::unique_ptr<Token>> &tokens);
  private:
    /// select a statement and run its handler
    std::unique_ptr<Node> get_statement(const std::vector<std::unique_ptr<Token>> &tokens);

    // handlers for different types of operators
    std::unique_ptr<Node> get_assign(const std::vector<std::unique_ptr<Token>> &tokens);
    std::unique_ptr<Node> get_input(const std::vector<std::unique_ptr<Token>> &tokens);

    std::unique_ptr<Node> get_print(const std::vector<std::unique_ptr<Token>> &tokens);

    std::unique_ptr<Node> get_if(const std::vector<std::unique_ptr<Token>> &tokens);
    std::unique_ptr<Node> get_while(const std::vector<std::unique_ptr<Token>> &tokens);

    ...
};
```
</details>

However, during development it turned out that there are tools for generating lexical and syntax analyzers: `Flex` and `Bison`. It was decided to use these tools because of their significant advantages:

- **Automation** — eliminates the need to implement the parser manually
- **Reliability** — reduces the number of errors in syntax analysis and increases robustness
- **Support for complex grammars** — built-in handling of operator precedence and associativity
- **Faster development** — grammar changes lead to immediate regeneration of the parser

Therefore, a description file for generating the lexical analyzer was written: [lexer.l](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/lexer.l).
It defines the following constructs:

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

and the rules for handling them:
```y
{WHITESPACE}    { /* skip blanks and tabs */ }
{NEWLINE}       { ++yylineno; }
{LINE_COMMENT}  { /* skip */ }
{BLOCK_COMMENT} { /* skip */ }

"if"            { return process_if();   }
"else"          { return process_else(); }
"while"         { return process_while(); }
"print"         { return process_print(); }
"?"             { return process_input(); }
...
```

As you can see in `lexer.l`, some functions are called to handle the rules. These functions are defined in the `Lexer` class, which inherits from
`yyFlexLexer` (see [lexer.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/lexer.hpp)).
They store values in the `current_lexem` and `current_value` fields of the class so that they can be printed while scanning a lexeme.
It is planned that this class will also be used for error handling. An example of a handler function:
```C++
int process_if() {
    current_lexem = "conditional operator";
    current_value = "if";
    return yy::parser::token::TOK_IF;
}
```

The return value is a token from the parser generated by `Bison`; this is done for joint work of `Bison` and `Flex`.

### Syntax analyzer implementation
To generate the syntax analyzer with `Bison`, the grammar description was written in the corresponding format: [parser.y](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/src/parser.y). 

The function that connects the parser with the lexer looks like this:
```C++
int yylex(yy::parser::semantic_type*   yylval,
          yy::parser::location_type*   yylloc,
          language::Lexer*             scanner)
{
    auto tt = scanner->yylex();

    if (tt == yy::parser::token::TOK_NUMBER)
      yylval->build<int>() = std::stoi(scanner->YYText());

    if (tt == yy::parser::token::TOK_ID)
      yylval->build<std::string>() = scanner->YYText();

    return tt;
}
```
For numbers and variables, the value is stored in `yylval`; in other cases the token type is simply returned.

During syntax analysis, an `AST` (abstract syntax tree) is built. 
By introducing additional rules for syntax analysis, the hierarchy of operator precedence is implemented.

The tree can also be visualized using graphviz. To build the image, run:
```bash
dot dump/graph_dump.gv -Tsvg -o dump/graph_dump.svg
```
and you will get a graphical representation of the tree (example):
<div align="center">
  <img src="img/graph_dump.svg" alt="Dump Banner" width="1200">
</div>

### Simulator
To simulate the execution of a program, a `Simulator` class was implemented [simulator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/simulator.hpp), which inherits from the abstract class `ASTVisitor`:

```C++
// Visitor pattern for AST traversal
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    virtual void visit(Program &node) = 0;
    virtual void visit(Block_stmt &node) = 0;
    virtual void visit(Assignment_stmt &node) = 0;
    virtual void visit(Input_stmt &node) = 0;
    virtual void visit(If_stmt &node) = 0;
...
};
```

In the `Simulator` class, the virtual functions of `ASTVisitor` are overridden and an expression evaluation function is introduced,
which uses a special `ExpressionEvaluator` class [expr_evaluator.hpp](https://github.com/RTCupid/Super_Biba_Boba_Language/blob/main/frontend/include/expr_evaluator.hpp):

```C++
number_t Simulator::evaluate_expression(Expression &expression) {
    ExpressionEvaluator evaluator(*this);
    expression.accept(evaluator);

    return evaluator.get_result();
}
```

`ExpressionEvaluator` is specialized only for expression evaluation. It contains the `result_` field to store the expression result, and `simulator_` — 
a reference to the simulator instance from which it was called, so that it has access to the symbol table.

### Example program
Below is a simple example of a correct program in the language — computing the n-th `Fibonacci number`:

```C
fst = 0;               // no type required, everything is int 
snd = 1;
iters = ?;             // read a number from std::cin and define the variable
while (iters > 1) {
    tmp = fst;
    fst = snd;
    snd = snd + tmp;
    iters = iters - 1;
}
print snd;             // print the value of the variable to std::cout
```

## 👥 Authors

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
1. Extended Backus–Naur Form (EBNF) [Electronic resource] (in Russian): article – https://divancoder.ru/2017/06/ebnf/ (accessed May 21, 2025).
