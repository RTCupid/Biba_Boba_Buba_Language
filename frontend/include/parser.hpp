#ifndef FRONTEND_INCLUDE_PARSER_HPP
#define FRONTEND_INCLUDE_PARSER_HPP

#include "node.hpp"
#include "token.hpp"
#include <memory>
#include <vector>
#include <stdexcept>
#include <cassert>

namespace language {

// Program     ::= StmtList
// StmtList    ::= Statement ';'
//
// Statement   ::= Assignment | IfStmt  | WhileStmt |  Input   | Print
//
// IfStmt      ::= 'if'    '(' Expression ')' '{' StmtList '}'
// WhileStmt   ::= 'while' '(' Expression ')' '{' StmtList '}'
//
// Assignment  ::= Var '=' Expression
// Input       ::= Var '=' '?'
//
// Print       ::= 'print' Expression
//
// Expression  ::= Equality
// Equality    ::= Rel ( ( '==' | '!=' ) Rel )*
// Rel         ::= AddSub ( ( '<' | '>' | '<=' | '>=' ) AddSub )*
// AddSub      ::= Primary ( ( '+' | '-' ) Primary )*
// Primary     ::= '(' Expression ')'| Var | Number
//
// Var         ::= ([a-z] | [A-Z])+
// Num         ::= ['0'-'9']+

class Parser {
    std::size_t pc_ = 0;
    std::unique_ptr<Node> root_{nullptr};

  public:
    Parser() = default;

    bool parse(const std::vector<std::unique_ptr<Token>> &tokens);

  private:
    /// select a statement and run its handler
    std::unique_ptr<Node> get_statement(const std::vector<std::unique_ptr<Token>> &tokens);

    // handlers for different types of tokens
    std::unique_ptr<Node> handle_binary_operator_case(const std::vector<std::unique_ptr<Token>> &tokens);
    std::unique_ptr<Node> handle_unary_operator_case(const std::vector<std::unique_ptr<Token>> &tokens);
    std::unique_ptr<Node> handle_conditional_operator_case(const std::vector<std::unique_ptr<Token>> &tokens);

    std::unique_ptr<Node> get_assign(const std::vector<std::unique_ptr<Token>> &tokens);
    std::unique_ptr<Node> get_input(const std::vector<std::unique_ptr<Token>> &tokens);

    std::unique_ptr<Node> get_print(const std::vector<std::unique_ptr<Token>> &tokens);

    std::unique_ptr<Node> get_if(const std::vector<std::unique_ptr<Token>> &tokens);
    std::unique_ptr<Node> get_while(const std::vector<std::unique_ptr<Token>> &tokens);

    inline void check_and_skip_semicolon(const std::vector<std::unique_ptr<Token>> &tokens);
};

inline bool Parser::parse(const std::vector<std::unique_ptr<Token>> &tokens) {
    auto root_ = std::make_unique<Statement_node>();

    root_->set_left(get_statement(tokens));
    root_->set_right(std::make_unique<Statement_node>());

    auto current_node = static_cast<Statement_node*>(root_->right_.get());
    if (!current_node) {
        throw std::runtime_error("Expected Statement_node");
    }

    while (pc_ < tokens.size()) {
        current_node->set_left(get_statement(tokens));
        current_node->set_right(std::make_unique<Statement_node>());
        current_node = static_cast<Statement_node*>(current_node->right_.get());
    }

    return true;
}

inline std::unique_ptr<Node> Parser::get_statement(const std::vector<std::unique_ptr<Token>> &tokens) {
    if (tokens[pc_]->type_ == Type::type_variable) {
        ++pc_;
        if (tokens[pc_]->type_ == Type::type_binary_operator) {
            return handle_binary_operator_case(tokens);
        } else {
            --pc_;
            throw std::runtime_error("Expected binary operator token");
        }
    } else if (tokens[pc_]->type_ == Type::type_unary_operator) {
        return handle_unary_operator_case(tokens);
    } else if (tokens[pc_]->type_ == Type::type_conditional_operator) {
        return handle_conditional_operator_case(tokens);
    } else {
        throw std::runtime_error("Expected statement");
    }
}

inline std::unique_ptr<Node> Parser::handle_binary_operator_case(const std::vector<std::unique_ptr<Token>> &tokens) {
    assert(pc_ > 0);

    auto* binary_op = dynamic_cast<Token_binary_operator*>(tokens[pc_].get());
    if (!binary_op)
        throw std::runtime_error("Expected binary operator token");

    if (binary_op->binary_operator_ == Binary_operators::operator_assign) {
        --pc_;
        auto assign_node = get_assign(tokens);
        check_and_skip_semicolon(tokens);
        return assign_node;
    } else if (binary_op->binary_operator_ == Binary_operators::operator_input) {
        --pc_;
        auto input_node = get_input(tokens);
        check_and_skip_semicolon(tokens);
        return input_node;
    } else {
        throw std::runtime_error("Unknown binary operator token");
    }
}

inline std::unique_ptr<Node> Parser::handle_unary_operator_case(const std::vector<std::unique_ptr<Token>> &tokens) {
    auto* unary_op = dynamic_cast<Token_unary_operator*>(tokens[pc_].get());
    if (!unary_op)
        throw std::runtime_error("Expected binary operator token");

    if (unary_op->unary_operator_ == Unary_operators::operator_print) {
        auto print_node = get_print(tokens);
        check_and_skip_semicolon(tokens);
        return print_node;
    } else {
        throw std::runtime_error("Unknown unary operator token");
    }
}

inline std::unique_ptr<Node> Parser::handle_conditional_operator_case(const std::vector<std::unique_ptr<Token>> &tokens) {
    auto* cond_op = dynamic_cast<Token_conditional_operator*>(tokens[pc_].get());
    if (!cond_op)
        throw std::runtime_error("Expected binary operator token");

    if (cond_op->conditional_operator_ == Conditional_operators::operator_if) {
        auto if_node = get_if(tokens);
        return if_node;
    } else if (cond_op->conditional_operator_ == Conditional_operators::operator_while) {
        auto while_node = get_while(tokens);

        return while_node;
    } else {
        throw std::runtime_error("Unknown conditional operator token");
    }
}

inline void Parser::check_and_skip_semicolon(const std::vector<std::unique_ptr<Token>> &tokens) {
    auto* identifier = dynamic_cast<Token_identifier*>(tokens[pc_].get());
    if (!identifier)
        throw std::runtime_error("Expected identifier token");
    if (!(identifier->identifier_ == Identifiers::id_semicolon))
        throw std::runtime_error("Expected semicolon identifier token");
    ++pc_;
}

} // namespace language

#endif // FRONTEND_INCLUDE_PARSER_HPP
