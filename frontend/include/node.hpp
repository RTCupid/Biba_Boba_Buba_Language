#ifndef FRONTEND_INCLUDE_AST_HPP
#define FRONTEND_INCLUDE_AST_HPP

#include <memory>
#include <vector>
#include <string>

namespace language {

using number_t = int;

class Node {
public:
    virtual ~Node() = default;
};

class Statement : public Node {};
class Expression : public Node {};

using Statement_ptr = std::unique_ptr<Statement>;
using StmtList = std::vector<Statement_ptr>;
using Expression_ptr = std::unique_ptr<Expression>;

class Program : public Node {
private:
    StmtList stmts;
public:
    explicit Program(StmtList s) : stmts(std::move(s)) {}
};

class Assignment : public Statement {
private:
    std::string var_name_;
    Expression_ptr value_;
public:
    Assignment(std::string var_name, Expression_ptr value) : var_name_(std::move(var_name)), value_(std::move(value)) {}
};

// class Binary_operator_node : public Node {
//     std::unique_ptr<Node> left_{nullptr};
//     std::unique_ptr<Node> right_{nullptr};

//     Binary_operator_node(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right)
//         : left_(std::move(left)), right_(std::move(right)) {}
// };

// class Unary_operator_node : public Node {
//     std::unique_ptr<Node> right_{nullptr};

//     Unary_operator_node(std::unique_ptr<Node> &&right) : right_(std::move(right)) {}
// };

// class Statement_node : public Node {
//   public:
//     std::unique_ptr<Node> left_{nullptr};
//     std::unique_ptr<Node> right_{nullptr};

//     Statement_node() = default;
//     Statement_node(std::unique_ptr<Node> &&left, std::unique_ptr<Node> &&right)
//         : left_(std::move(left)), right_(std::move(right)) {}

//     void set_left(std::unique_ptr<Node> &&left) {
//         left_ = std::move(left);
//     }

//     void set_right(std::unique_ptr<Node> &&right) {
//         right_ = std::move(right);
//     }
// };

class Number: public Node {
private:
    number_t number_;
public:
    explicit Number(number_t number) : number_(number) {}
};

class Var : public Node {
private:
    std::string var_name_;
public:
    explicit Var(std::string var_name) : var_name_(std::move(var_name)) {}
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_HPP
