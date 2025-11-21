#include "simulator.hpp"
#include "expr_evaluator.hpp"
#include "node.hpp"
#include <iostream>
#include <unordered_map>

namespace language {

void Simulator::visit(Program &node) {
    const auto &statements = node.get_stmts();

    for (const auto &stmt : statements) {
        stmt->accept(*this);
    }
}

void Simulator::visit(Block_stmt &node) {
    const auto &statements = node.get_stmts();

    for (const auto &stmt : statements) {
        stmt->accept(*this);
    }
};

void Simulator::visit(Assignment_stmt &node) {
    const auto &var_name = node.get_variable()->get_name();
    const auto &value = evaluate_expression(node.get_value());

    auto it = nametable.find(var_name);
    if (it != nametable.end())
        it->second = value;
    else
        nametable.emplace(var_name, value);
};

void Simulator::visit(Input_stmt &node) {
    const auto &var_name = node.get_variable()->get_name();
    number_t value;
    std::cin >> value;

    auto it = nametable.find(var_name);
    if (it != nametable.end())
        it->second = value;
    else
        nametable.emplace(var_name, value);
};

void Simulator::visit(If_stmt &node) {
    auto condition = evaluate_expression(node.get_condition());

    if (condition != 0) {
        node.then_branch().accept(*this);
    } else {
        const bool contains_else_node = node.contains_else_branch();

        if (contains_else_node)
            node.else_branch().accept(*this);
    }
};

void Simulator::visit(While_stmt &node) {
    while (evaluate_expression(node.get_condition())) {
        node.get_body().accept(*this);
    }
};

void Simulator::visit(Print_stmt &node) {
    auto value = evaluate_expression(node.get_value());

    std::cout << value << '\n';
};

void Simulator::visit(Assignment_expr &node) {}
void Simulator::visit(Binary_operator &node) {}
void Simulator::visit(Unary_operator &node) {}
void Simulator::visit(Number &node) {}
void Simulator::visit(Variable &node) {}

number_t Simulator::evaluate_expression(Expression &expression) {
    ExpressionEvaluator evaluator(*this);
    expression.accept(evaluator);
    return evaluator.get_result();
}

} // namespace language
