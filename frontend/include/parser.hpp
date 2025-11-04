#ifndef FRONTEND_INCLUDE_PARSER_HPP
#define FRONTEND_INCLUDE_PARSER_HPP

#include "node.hpp"
#include "token.hpp"
#include <vector>
#include <memory>

namespace language {

class Parser {
    std::size_t pc_ = 0;
    std::unique_ptr<Node> root{nullptr};

public:
    Parser () = default;


    bool parse(const std::vector<std::unique_ptr<Token>>& tokens_);

private:
    Node* get_operator() {

    }

};

inline bool Parser::parse(const std::vector<std::unique_ptr<Token>> &tokens_) {
    auto node = new Statement_node{get_operator(), new Statement_node};

    auto current_node = node->right_;

    while (true) {


    }

    return node;
}

} // namespace language

#endif // FRONTEND_INCLUDE_PARSER_HPP
