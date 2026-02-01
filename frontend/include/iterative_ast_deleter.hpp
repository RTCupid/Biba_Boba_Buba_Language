#ifndef FRONTEND_INCLUDE_ITERATIVE_AST_DELETER_HPP
#define FRONTEND_INCLUDE_ITERATIVE_AST_DELETER_HPP

#include "node.hpp"
#include <memory>
#include <utility>
#include <vector>

namespace language {

struct Iterative_ast_deleter {
    void operator()(Node *root) const {
        if (!root) {
            return;
        }

        std::vector<std::unique_ptr<Node>> stack;
        stack.emplace_back(root);

        while (!stack.empty()) {
            auto node = std::move(stack.back());
            stack.pop_back();

            node->detach_children(stack);
        }
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_ITERATIVE_AST_DELETER_HPP