#include "iterative_ast_deleter.hpp"

namespace language {

void Program::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    for (auto &stmt : stmts_) {
        if (stmt) {
            stack.emplace_back(std::move(stmt));
        }
    }
    stmts_.clear();
}

void Empty_stmt::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    // No children to detach
}

void Block_stmt::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    for (auto &stmt : stmts_) {
        if (stmt) {
            stack.emplace_back(std::move(stmt));
        }
    }
    stmts_.clear();
}

void Assignment_stmt::detach_children(
    std::vector<std::unique_ptr<Node>> &stack) {
    if (variable_) {
        stack.emplace_back(std::move(variable_));
    }
    if (value_) {
        stack.emplace_back(std::move(value_));
    }
}

void Assignment_expr::detach_children(
    std::vector<std::unique_ptr<Node>> &stack) {
    if (variable_) {
        stack.emplace_back(std::move(variable_));
    }
    if (value_) {
        stack.emplace_back(std::move(value_));
    }
}

void While_stmt::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    if (condition_) {
        stack.emplace_back(std::move(condition_));
    }
    if (body_) {
        stack.emplace_back(std::move(body_));
    }
}

void If_stmt::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    if (condition_) {
        stack.emplace_back(std::move(condition_));
    }
    if (then_branch_) {
        stack.emplace_back(std::move(then_branch_));
    }
    if (else_branch_) {
        stack.emplace_back(std::move(else_branch_));
    }
}

void Input::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    // No children to detach
}

void Print_stmt::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    if (value_) {
        stack.emplace_back(std::move(value_));
    }
}

void Binary_operator::detach_children(
    std::vector<std::unique_ptr<Node>> &stack) {
    if (left_) {
        stack.emplace_back(std::move(left_));
    }
    if (right_) {
        stack.emplace_back(std::move(right_));
    }
}

void Unary_operator::detach_children(
    std::vector<std::unique_ptr<Node>> &stack) {
    if (operand_) {
        stack.emplace_back(std::move(operand_));
    }
}

void Number::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    // No children to detach
}

void Variable::detach_children(std::vector<std::unique_ptr<Node>> &stack) {
    // No children to detach
}

} // namespace language