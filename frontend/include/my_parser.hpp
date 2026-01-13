#ifndef FRONTEND_INCLUDE_MY_PARSER_HPP
#define FRONTEND_INCLUDE_MY_PARSER_HPP

#include "parser.hpp"
#include "error_collector.hpp"
#include "lexer.hpp"
#include <memory.h>

namespace language {

using nametable_t = std::unordered_map<language::name_t, bool /*defined*/>;

class My_parser : public yy::parser {
private:
    Lexer* scanner_;
    Scope scopes_;
    std::unique_ptr<Program> root_;
public:
    Error_collector error_collector_;

    My_parser(Lexer* scanner, std::unique_ptr<language::Program> &root) : yy::parser(scanner, root, this), scanner_(scanner), root_(std::move(root)) {}

    void push_scope(nametable_t &nametable) {
        scopes_.push(nametable);
    }

    void pop_scope() {
        scopes_.pop();
    }

    bool find_in_scopes(std::string &var_name) const {
        return scopes_.find(var_name);
    }

    void add_var_to_scope(std::string &var_name, bool defined) {
        scopes_.add_variable(var_name, defined);
    }
};

} //namespace language

#endif // FRONTEND_INCLUDE_MY_PARSER_HPP
