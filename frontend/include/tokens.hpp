#ifndef FRONTEND_INCLUDE_TOKENS_HPP
#define FRONTEND_INCLUDE_TOKENS_HPP

namespace language {

enum class TokenType {
    TOK_EOF = 0,
    TOK_IF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_PRINT,
    TOK_INPUT,
    TOK_PLUS,
    TOK_MINUS,
    TOK_MUL,
    TOK_DIV,
    TOK_ASSIGN,
    TOK_EQ,
    TOK_NOT_EQ,
    TOK_LESS,
    TOK_GREATER,
    TOK_LESS_OR_EQ,
    TOK_GREATER_OR_EQ,
    TOK_LEFT_PAREN,
    TOK_RIGHT_PAREN,
    TOK_LEFT_BRACE,
    TOK_RIGHT_BRACE,
    TOK_SEMICOLON,
    TOK_ID,
    TOK_NUMBER,
};

} // namespace language

#endif // FRONTEND_INCLUDE_TOKENS_HPP
