#ifndef FRONTEND_INCLUDE_TOKEN_HPP
#define FRONTEND_INCLUDE_TOKEN_HPP

#include "common.hpp"
#include <string>

namespace language {

struct Token {
    Type type_{Type::type_zero};
    Token() = default;
    explicit Token(Type t) : type_{t} {}
    virtual ~Token() = default;
};

struct Token_binary_operator : public Token {
    Binary_operators binary_operator_{Binary_operators::operator_zero};
    
    Token_binary_operator()
        : Token(Type::type_binary_operator) {}

    explicit Token_binary_operator(Binary_operators op)
        : Token(Type::type_binary_operator),
          binary_operator_(op) {}

};

struct Token_unary_operator : public Token {
    Unary_operators unary_operator_{Unary_operators::operator_zero};

    Token_unary_operator()
        : Token(Type::type_unary_operator) {}

    explicit Token_unary_operator(Unary_operators op)
        : Token(Type::type_unary_operator),
          unary_operator_(op) {}
};

struct Token_conditional_operator : public Token {
    Conditional_operators conditional_operator_{Conditional_operators::empty_operator};
};

struct Token_number : public Token {
    number_type number_{nil};

    Token_number() : Token(Type::type_number) {}

    explicit Token_number(number_type n)
        : Token(Type::type_number), number_{n} {}
};

struct Token_identifier : public Token {
    Identifiers identifier_{Identifiers::id_zero};

    explicit Token_identifier(Identifiers id)
        : Token(Type::type_identifier),
          identifier_(id) {}
};

struct Token_variable : public Token {
    std::string variable_name{};
    number_type variable_value{nil};

    Token_variable() : Token(Type::type_variable) {}
    Token_variable(std::string name, number_type value = nil)
        : Token(Type::type_variable),
          variable_name(std::move(name)),
          variable_value(value) {}
};

} // namespace language


#endif // TREE_INCLUDE_TOKEN_HPP
