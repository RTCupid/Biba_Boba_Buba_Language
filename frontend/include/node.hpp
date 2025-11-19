#ifndef FRONTEND_INCLUDE_AST_HPP
#define FRONTEND_INCLUDE_AST_HPP

#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace language {

using number_t = int;

// Forward declarations
class Node;
class Program;
class Statement;
class Expression;
class Assignment_stmt;
class Block_stmt;
class If_stmt;
class While_stmt;
class Print_stmt;
class Unary;
class Number;
class Variable;
class Input_stmt;
class Binary_operator;
class Unary_operator;

// Visitor pattern for AST traversal
class ASTVisitor {
  public:
    virtual ~ASTVisitor() = default;

    virtual void visit(Program &node) = 0;
    virtual void visit(Block_stmt &node) = 0;
    virtual void visit(Assignment_stmt &node) = 0;
    virtual void visit(Input_stmt &node) = 0;
    virtual void visit(If_stmt &node) = 0;
    virtual void visit(While_stmt &node) = 0;
    virtual void visit(Print_stmt &node) = 0;
    virtual void visit(Binary_operator &node) = 0;
    virtual void visit(Unary_operator &node) = 0;
    virtual void visit(Number &node) = 0;
    virtual void visit(Variable &node) = 0;
};

class Node {
  public:
    virtual ~Node() = default;
    virtual void accept(ASTVisitor &visitor) = 0;
    virtual void graph_dump(std::ostream &gv, Node *parent) const = 0; // TODO
};

enum class Binary_operators {
    Eq,
    Neq,
    Less,
    LessEq,
    Greater,
    GreaterEq,
    Add,
    Sub,
    Mul,
    Div,
    RemDiv
};

enum class Unary_operators { Neg, Plus, Not };

class Statement : public Node {};
class Expression : public Node {};

using Statement_ptr = std::unique_ptr<Statement>;
using StmtList = std::vector<Statement_ptr>;
using Expression_ptr = std::unique_ptr<Expression>;
using Variable_ptr = std::unique_ptr<Variable>;

class Program : public Node {
  private:
    StmtList stmts_;

  public:
    explicit Program(StmtList stmts) : stmts_(std::move(stmts)) {}

    const StmtList &get_stmts() const { return stmts_; }
    StmtList &get_stmts() { return stmts_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Block_stmt : public Statement {
  private:
    StmtList stmts_;

  public:
    explicit Block_stmt(StmtList stmts) : stmts_(std::move(stmts)) {}

    const StmtList &get_stmts() const { return stmts_; }
    StmtList &get_stmts() { return stmts_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Assignment_stmt : public Statement {
  private:
    Variable_ptr variable_;
    Expression_ptr value_;

  public:
    Assignment_stmt(Variable_ptr variable, Expression_ptr value)
        : variable_(std::move(variable)), value_(std::move(value)) {}

    const Variable_ptr &get_variable() const { return variable_; }
    Expression &get_value() { return *value_; }
    const Expression &get_value() const { return *value_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class While_stmt : public Statement {
  private:
    Expression_ptr condition_;
    Statement_ptr body_;

  public:
    While_stmt(Expression_ptr condition, Statement_ptr body)
        : condition_(std::move(condition)), body_(std::move(body)) {}

    Expression &get_condition() { return *condition_; }
    Statement &get_body() { return *body_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class If_stmt : public Statement {
  private:
    Expression_ptr condition_;
    Statement_ptr then_branch_;
    Statement_ptr else_branch_;

  public:
    If_stmt(Expression_ptr condition, Statement_ptr then_branch,
            Statement_ptr else_branch = nullptr)
        : condition_(std::move(condition)), then_branch_(std::move(then_branch)),
          else_branch_(std::move(else_branch)) {}

    Expression &get_condition() { return *condition_; }
    Statement &then_branch() { return *then_branch_; }
    Statement &else_branch() { return *else_branch_; }
    bool contains_else_branch() const { return else_branch_.get(); }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Input_stmt : public Statement {
  private:
    Variable_ptr variable_;

  public:
    explicit Input_stmt(Variable_ptr variable) : variable_(std::move(variable)) {}

    const Variable_ptr &get_variable() const { return variable_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Print_stmt : public Statement {
  private:
    Expression_ptr value_;

  public:
    explicit Print_stmt(Expression_ptr value) : value_(std::move(value)) {}

    Expression &get_value() { return *value_; }
    const Expression &get_value() const { return *value_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Binary_operator : public Expression {
  private:
    Binary_operators op_;
    Expression_ptr left_;
    Expression_ptr right_;

  public:
    Binary_operator(Binary_operators op, Expression_ptr left, Expression_ptr right)
        : op_(op), left_(std::move(left)), right_(std::move(right)) {}

    Binary_operators get_operator() const { return op_; }
    Expression &get_left() { return *left_; }
    const Expression &get_left() const { return *left_; }
    Expression &get_right() { return *right_; }
    const Expression &get_right() const { return *right_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Unary_operator : public Expression {
  private:
    Unary_operators op_;
    Expression_ptr operand_;

  public:
    Unary_operator(Unary_operators op, Expression_ptr operand)
        : op_(op), operand_(std::move(operand)) {}

    Unary_operators get_operator() const { return op_; }
    Expression &get_operand() { return *operand_; }
    const Expression &get_operand() const { return *operand_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Number : public Expression {
  private:
    number_t number_;

  public:
    explicit Number(number_t number) : number_(number) {}
    number_t &get_value() { return number_; }
    const number_t &get_value() const { return number_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

class Variable : public Expression {
  private:
    std::string var_name_;

  public:
    explicit Variable(std::string var_name) : var_name_(std::move(var_name)) {}

    std::string &get_name() { return var_name_; }
    const std::string &get_name() const { return var_name_; }

    void accept(ASTVisitor &visitor) override { visitor.visit(*this); }

    virtual void graph_dump(std::ostream &gv, Node *parent) const override;
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_HPP
