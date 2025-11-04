#ifndef FRONTEND_INCLUDE_NODE_HPP
#define FRONTEND_INCLUDE_NODE_HPP

#include "token.hpp"
#include <memory>

namespace language {

class Node {
};

class Binary_operation_node : public Node, public Token_binary_operator {
    std::unique_ptr<Node> left_{nullptr};
    std::unique_ptr<Node> right_{nullptr};

    Binary_operation_node (Node *left, Node *right) : left_(left), right_(right) {}
};

class Unary_operation_node : public Node, public Token_unary_operator {
    std::unique_ptr<Node> left_{nullptr};
};

class Statement_node : public Node {
public:
    std::unique_ptr<Node> left_{nullptr};
    std::unique_ptr<Node> right_{nullptr};

    Statement_node () = default;
    Statement_node (std::unique_ptr<Node>&& left, std::unique_ptr<Node>&& right) : left_(left), right_(right) {}
};

class Number_node : public Node, public Token_number {
};

class Variable_node : public Node, public Token_variable {
};


} // namespace language

#endif // TREE_INCLUDE_NODE_HPP
