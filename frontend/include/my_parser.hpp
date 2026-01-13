#ifndef FRONTEND_INCLUDE_MY_PARSER_HPP
#define FRONTEND_INCLUDE_MY_PARSER_HPP

#include "error_collector.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <memory.h>

namespace language {

using nametable_t = std::unordered_map<language::name_t, bool /*defined*/>;

class My_parser final : public yy::parser {
  private:
    Lexer *scanner_;
    std::unique_ptr<Program> root_;

  public:
    Error_collector error_collector;
    Scope scopes;

    My_parser(Lexer *scanner, std::unique_ptr<language::Program> &root, const std::string &program_file)
        : yy::parser(scanner, root, this), scanner_(scanner),
          root_(std::move(root)), error_collector(program_file) {}
};

} // namespace language

#endif // FRONTEND_INCLUDE_MY_PARSER_HPP
