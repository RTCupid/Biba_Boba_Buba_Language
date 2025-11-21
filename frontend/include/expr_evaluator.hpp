#ifndef FRONTEND_INCLUDE_EXPR_EVALUATOR_HPP
#define FRONTEND_INCLUDE_EXPR_EVALUATOR_HPP

#include "simulator.hpp"

namespace language {

class ExpressionEvaluator final : public ASTVisitor {
  private:
    Simulator &simulator_;
    number_t result_{0};

  public:
    ExpressionEvaluator(Simulator &simulator) : simulator_{simulator} {};
    ~ExpressionEvaluator() = default;

    number_t get_result() const;

    void visit(Number &node) override;

    void visit(Variable &node) override;

    void visit(Binary_operator &node) override;

    void visit(Unary_operator &node) override;

    void visit(Assignment_expr &node) override;

    void visit(Program &node) override;
    void visit(Block_stmt &node) override;
    void visit(Assignment_stmt &node) override;
    void visit(Input_stmt &node) override;
    void visit(If_stmt &node) override;
    void visit(While_stmt &node) override;
    void visit(Print_stmt &node) override;
};

} // namespace language

#endif // FRONTEND_INCLUDE_EXPR_EVALUATOR_HPP
