#ifndef FRONTEND_INCLUDE_TOKEN_HPP
#define FRONTEND_INCLUDE_TOKEN_HPP

#include "common.hpp"
#include <string>

namespace language {

struct Token {
    Type type_{Type::type_zero};
};

struct Token_binary_operator : public Token {
    Binary_operators binary_operator_{Binary_operators::operator_zero};
};

struct Token_unary_operator : public Token {
    Unary_operators unary_operator_{Unary_operators::operator_zero};
};

struct Token_conditional_operator : public Token {
    Conditional_operators conditional_operator_{Conditional_operators::empty_operator};
};

struct Token_number : public Token {
    number_type number_{nil};
};

struct Token_variable : public Token {
    std::string variable_name{};
    number_type variable_value{nil};
};









} // namespace language


#endif // TREE_INCLUDE_TOKEN_HPP
