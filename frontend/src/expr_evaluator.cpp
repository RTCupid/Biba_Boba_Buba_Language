#include "expr_evaluator.hpp"
#include "simulator.hpp"
#include <iostream>
#include <string>

namespace language {

number_t ExpressionEvaluator::get_result() const { return result_; }

void ExpressionEvaluator::visit(Number &node) { result_ = node.get_value(); }

void ExpressionEvaluator::visit(Variable &node) {
    std::string var_name = node.get_name();

    auto it = simulator_.nametable.find(var_name);
    if (it != simulator_.nametable.end()) {
        result_ = it->second;
    } else {
        throw std::runtime_error("Unknown variable: " + var_name);
    }
}

void ExpressionEvaluator::visit(Assignment_expr &node) {
    const auto &var_name = node.get_variable()->get_name();

    ExpressionEvaluator result_eval{simulator_};
    node.get_value().accept(result_eval);
    result_ = result_eval.result_;

    auto it = simulator_.nametable.find(var_name);
    if (it != simulator_.nametable.end())
        it->second = result_;
    else
        simulator_.nametable.emplace(var_name, result_);
};

void ExpressionEvaluator::visit(Binary_operator &node) {
    ExpressionEvaluator left_eval{simulator_};
    node.get_left().accept(left_eval);
    auto left_value = left_eval.result_;

    ExpressionEvaluator right_eval{simulator_};
    node.get_right().accept(right_eval);
    auto right_value = right_eval.result_;

    switch (node.get_operator()) {
    case Binary_operators::Eq: {
        result_ = (left_value == right_value);
        break;
    }
    case Binary_operators::Neq: {
        result_ = (left_value != right_value);
        break;
    }
    case Binary_operators::Less: {
        result_ = (left_value < right_value);
        break;
    }
    case Binary_operators::LessEq: {
        result_ = (left_value <= right_value);
        break;
    }
    case Binary_operators::Greater: {
        result_ = (left_value > right_value);
        break;
    }
    case Binary_operators::GreaterEq: {
        result_ = (left_value >= right_value);
        break;
    }
    case Binary_operators::Add: {
        result_ = left_value + right_value;
        break;
    }
    case Binary_operators::Sub: {
        result_ = left_value - right_value;
        break;
    }
    case Binary_operators::Mul: {
        result_ = left_value * right_value;
        break;
    }
    case Binary_operators::Div: {
        result_ = left_value / right_value;
        break;
    }
    case Binary_operators::RemDiv: {
        result_ = left_value % right_value;
        break;
    }
    default:
        throw std::runtime_error("Unknown binary operator");
    }
}

void ExpressionEvaluator::visit(Unary_operator &node) {
    ExpressionEvaluator eval{simulator_};
    node.get_operand().accept(eval);
    auto value = eval.result_;
    switch (node.get_operator()) {
    case Unary_operators::Neg: {
        result_ = -(value);
        break;
    }
    case Unary_operators::Plus: {
        result_ = value;
        break;
    }
    case Unary_operators::Not: {
        result_ = !value;
        break;
    }
    default:
        throw std::runtime_error("Unknown unary operator");
    }
}

void ExpressionEvaluator::visit(Input &node) {
    number_t value;
    std::cin >> value;

    result_ = value;
}

void ExpressionEvaluator::visit(Program &node) {}
void ExpressionEvaluator::visit(Block_stmt &node) {}
void ExpressionEvaluator::visit(Empty_stmt &node) {}
void ExpressionEvaluator::visit(Assignment_stmt &node) {}
void ExpressionEvaluator::visit(If_stmt &node) {}
void ExpressionEvaluator::visit(While_stmt &node) {}
void ExpressionEvaluator::visit(Print_stmt &node) {}

} // namespace language
