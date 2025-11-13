#ifndef FRONTEND_INCLUDE_AST_HPP
#define FRONTEND_INCLUDE_AST_HPP

#include <memory>
#include <string>
#include <vector>

namespace language {

using number_t = int;

// Forward declarations
class Node;
class Statement;
class Expression;
class Program;
class Assignment;
class BinaryOp;
class UnaryOp;
class Number;
class Variable;

// Visitor pattern for AST traversal
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;
    virtual void visit(Program &node) = 0;
    virtual void visit(Assignment &node) = 0;
    virtual void visit(BinaryOp &node) = 0;
    virtual void visit(UnaryOp &node) = 0;
    virtual void visit(Number &node) = 0;
    virtual void visit(Variable &node) = 0;
};

class Node {
  public:
    virtual ~Node() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
};

class Statement : public Node {};
class Expression : public Node {};
class LValueExpr : public Expression {};
class RValueExpr : public Expression {};

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
    Assignment(std::string var_name, Expression_ptr value)
        : var_name_(std::move(var_name)), value_(std::move(value)) {}

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

class Number : public RValueExpr {
  private:
    number_t number_;

  public:
    explicit Number(number_t number) : number_(number) {}

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

class Variable : public LValueExpr {
  private:
    std::string var_name_;

  public:
    explicit Variable(std::string var_name) : var_name_(std::move(var_name)) {}

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_HPP

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
