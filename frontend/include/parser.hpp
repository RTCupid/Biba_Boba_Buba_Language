#ifndef FRONTEND_INCLUDE_PARSER_HPP
#define FRONTEND_INCLUDE_PARSER_HPP

#include "node.hpp"
#include <vector>

namespace language {

class Parser {
    std::vector<Token> tokens_;
    size_t pc_ = 0;

public:
    Parser (const std::vector<Token> &tokens) : tokens_(tokens) {}

    Node *parse();

private:
    Node *get_operator() {

    }

};

inline Node *Parser::parse() {
    auto node = new Statement_node;

    node->left_ = get_operator();
    node->right_ = new Statement_node;

    auto current_node = node->right_;

    while (true) {


    }

    return node;
}

} // namespace language

#endif
