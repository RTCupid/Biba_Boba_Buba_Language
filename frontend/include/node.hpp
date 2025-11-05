#ifndef FRONTEND_INCLUDE_NODE_HPP
#define FRONTEND_INCLUDE_NODE_HPP

#include "token.hpp"
#include <memory>

namespace language {

class Node {};

class Binary_operator_node : public Node, public Token_binary_operator {
    std::unique_ptr<Node> left_{nullptr};
    std::unique_ptr<Node> right_{nullptr};

    Binary_operator_node(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right)
        : left_(std::move(left)), right_(std::move(right)) {}
};

class Unary_operator_node : public Node, public Token_unary_operator {
    std::unique_ptr<Node> right_{nullptr};

    Unary_operator_node(std::unique_ptr<Node> &&right) : right_(std::move(right)) {}
};

class Statement_node : public Node {
  public:
    std::unique_ptr<Node> left_{nullptr};
    std::unique_ptr<Node> right_{nullptr};

    Statement_node() = default;
    Statement_node(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right)
        : left_(std::move(left)), right_(std::move(right)) {}

    void set_left(std::unique_ptr<Node> &&left) {
        left_ = std::move(left);
    }

    void set_right(std::unique_ptr<Node> &&right) {
        right_ = std::move(right);
    }
};

class Number_node : public Node, public Token_number {};

class Variable_node : public Node, public Token_variable {};

} // namespace language

#endif // TREE_INCLUDE_NODE_HPP
