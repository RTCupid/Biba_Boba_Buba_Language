#include "simulator.hpp"
#include "expr_evaluator.hpp"
#include "node.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

namespace language {

void Simulator::visit(Program &node) {
    const auto &statements = node.get_stmts();

    for (const auto &stmt : statements) {
        stmt->accept(*this);
    }
}

void Simulator::visit(Block_stmt &node) {
    // std::cout << "Run block\n";

    const auto &statements = node.get_stmts();

    for (const auto &stmt : statements) {
        stmt->accept(*this);
    }
};

void Simulator::visit(Assignment_stmt &node) {
    // std::cout << "Run assignment\n";
    const auto &var_name = node.get_variable()->get_name();
    const auto &value = evaluate_expression(node.get_value());

    auto it = nametable.find(var_name);
    if (it != nametable.end()) {
        it->second = value;
        // std::cout << "variable " << var_name << " exsists in nametable, new value " << it->second
        // << '\n';
    } else {
        nametable.emplace(var_name, value);
        // std::cout << "Variable " << var_name << " created with value " << value << '\n';
    }
};

void Simulator::visit(Input_stmt &node) {
    // std::cout << "Run input\n";

    const auto &var_name = node.get_variable()->get_name();
    number_t value;
    std::cin >> value;

    auto it = nametable.find(var_name);
    if (it != nametable.end()) {
        it->second = value;
        // std::cout << "variable " << var_name << " exsists in nametable, new value " << it->second
        // << '\n';
    } else {
        nametable.emplace(var_name, value);
        // std::cout << "Variable " << var_name << " created with value " << value << '\n';
    }
};

void Simulator::visit(If_stmt &node) {
    // std::cout << "Run if\n";

    auto condition = evaluate_expression(node.get_condition());
    // std::cout << "step 1\n";
    // std::cout << "condition = " << condition << "\n";

    if (condition != 0) {
        // std::cout << "step 2\n";
        node.then_branch().accept(*this);
    } else {
        // std::cout << "step 3\n";
        const bool contains_else_node = node.contains_else_branch(); 

        if (contains_else_node)
            node.else_branch().accept(*this);
    }
    // std::cout << "step 10\n";
};

void Simulator::visit(While_stmt &node) {
    // std::cout << "Run while\n";

    while (evaluate_expression(node.get_condition())) {
        node.get_body().accept(*this);
    }
};

void Simulator::visit(Print_stmt &node) {
    // std::cout << "Run print\n";

    auto value = evaluate_expression(node.get_value());

    std::cout << value << '\n';
};

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
