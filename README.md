<div align="center">

# Implementation of the Super Biba-Boba programming language in C++
  ![C++](https://img.shields.io/badge/C++-23-blue?style=for-the-badge&logo=cplusplus)
  ![CMake](https://img.shields.io/badge/CMake-3.20+-green?style=for-the-badge&logo=cmake)
  ![Testing](https://img.shields.io/badge/Google_Test-Framework-red?style=for-the-badge&logo=google)

</div>

- This project is our implementation of programming language ParaCL from C++ cource by K.I.Vladimirov.

## 📝 Description of the grammar

Below is a description of the grammar of the language in a format close to EBNF [2], which was updated before the addition of the new syntax of the language:

```
Program        ::= StmtList EOF

StmtList       ::= /* empty */ | StmtList Statement

Statement      ::= AssignmentStmt ';' | InputStmt ';' | IfStmt | WhileStmt | PrintStmt ';' | BlockStmt

BlockStmt      ::= '{' StmtList '}'
AssignmentStmt ::= Var '=' Expression
InputStmt      ::= Var '=' '?'
IfStmt         ::= 'if'    '(' Expression ')' Statement [ 'else' Statement ]
WhileStmt      ::= 'while' '(' Expression ')' Statement
PrintStmt      ::= 'print' Expression

Expression     ::= Equality
Equality       ::= Relational ( ( '==' | '!=' ) Relational )*
Relational     ::= AddSub ( ( '<' | '>' | '<=' | '>=' ) AddSub )*
AddSub         ::= MulDiv ( ( '+' | '-' ) MulDiv )*
MulDiv         ::= Unary  ( ( '*' | '/' ) Unary )*
Unary          ::= '-' Unary | Primary
Primary        ::= '(' Expression ')' | Var | Number

Var            ::= [A-Za-z_][A-Za-z0-9_]*
Number         ::= [0-9]+
EOF            ::= __end_of_file__
```

## 🛠 Tool Selection: Flex and Bison
At the first stage of research into this problem, the design of token, node, lexer and parser classes began:
<details>
<summary>Show/Hide code</summary>
  
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

However, during the development process, we discovered tools for generating `lexical` and `syntactic` analyzers, such as `flex` and `bison`. We decided to employ these tools due to their significant advantages:

### ✅ Advantages
- **🤖 Automation** — eliminates the need for manual parser implementation

- **🛡 Reliability** — reduced parsing errors and increased robustness

- **🎯 Complex Grammar Support** — built-in handling of operator precedence and associativity

- **⚡ Rapid Development** — grammar modifications lead to immediate parser regeneration


## 👥 Project Creators

<div align="center">

  <a href="https://github.com/RTCupid">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/A.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <a href="https://github.com/BulgakovDmitry">
    <img src="https://raw.githubusercontent.com/BulgakovDmitry/3D_triangles/main/img/D.jpeg" width="160" height="160" style="border-radius: 50%;">
  </a>
  <br>
  <a href="https://github.com/RTCupid"><strong>@RTCupid, </strong></a>
  <a href="https://github.com/BulgakovDmitry"><strong>@BulgakovDmitry</strong></a>
  <br>
</div>

## 📚 References
1. Extended Backus-Naur Form [Electronic resource]: article. -  https://divancoder.ru/2017/06/ebnf/ (accessed May 21, 2025)
