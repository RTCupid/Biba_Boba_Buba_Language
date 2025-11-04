#ifndef FRONTEND_INCLUDE_NODE_HPP
#define FRONTEND_INCLUDE_NODE_HPP

#include "token.hpp"

namespace language {

class Node {
    Node* parent_{nullptr};
};

class Binary_operation_node : public Node, public Token_binary_operator {
    Node* left_{nullptr};
    Node* right_{nullptr};

    Binary_operation_node (Node *left, Node *right) : left_(left), right_(right) {}
};

class Unary_operation_node : public Node, public Token_unary_operator {
    Node* left_{nullptr};
};

class Statement_node : public Node {
public:
    Node* left_{nullptr};
    Node* right_{nullptr};

    Statement_node () = default;
    Statement_node (Node *left, Node *right) : left_(left), right_(right) {}
};

class Number_node : public Node, public Token_number {
};

class Variable_node : public Node, public Token_variable {
};


} // namespace language

#endif // TREE_INCLUDE_NODE_HPP
