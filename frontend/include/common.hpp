#ifndef FRONTEND_INCLUDE_COMMON_HPP
#define FRONTEND_INCLUDE_COMMON_HPP

namespace language {

using number_type = int;

inline constexpr number_type nil = 0;

enum class Type {
    type_zero,
    type_number,
    type_binary_operator,
    type_unary_operator,
    type_variable,
    type_identifier,
    type_conditional_operator,
    number_of_types
};

enum class Identifiers {
    id_zero,
    id_semicolon,
    id_left_parenthesis,
    id_right_parenthesis,
    id_left_curly_bracket,
    id_right_curly_bracket,
};

enum class Binary_operators {
    operator_zero,
    operator_add,
    operator_sub,
    operator_mul,
    operator_input,
    operator_div,
    operator_pow,
    operator_assign,
    number_of_operators
};

enum class Unary_operators {
    operator_zero,
    operator_unary_minus,
    operator_print,
    number_of_operators
};

enum class Conditional_operators {
    empty_operator,
    operator_while,
    operator_if,
    number_of_operators
};

} // namespace language

#endif // TREE_INCLUDE_COMMON_HPP
