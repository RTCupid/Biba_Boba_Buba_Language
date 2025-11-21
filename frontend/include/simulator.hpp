#ifndef FRONTEND_INCLUDE_SIMULATOR_HPP
#define FRONTEND_INCLUDE_SIMULATOR_HPP

#include "node.hpp"
#include <string>
#include <unordered_map>

namespace language {

class Simulator final : public ASTVisitor {
  public:
    std::unordered_map<std::string, number_t> nametable;
    Simulator() = default;
    ~Simulator() = default;

    void visit(Program &node) override;
    void visit(Block_stmt &node) override;

    void visit(Assignment_stmt &node) override;

    void visit(Input_stmt &node) override;

    void visit(If_stmt &node) override;

    void visit(While_stmt &node) override;

    void visit(Print_stmt &node) override;

    void visit(Assignment_expr &node) override;
    void visit(Binary_operator &node) override;
    void visit(Unary_operator &node) override;
    void visit(Number &node) override;
    void visit(Variable &node) override;

  private:
    number_t evaluate_expression(Expression &expression);
};

} // namespace language

#endif // FRONTEND_INCLUDE_SIMULATOR_HPP
