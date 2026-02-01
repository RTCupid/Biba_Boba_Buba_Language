#ifndef FRONTEND_INCLUDE_MY_PARSER_HPP
#define FRONTEND_INCLUDE_MY_PARSER_HPP

#include "config.hpp"
#include "error_collector.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <memory.h>
#include <unordered_set>

namespace language {

class My_parser final : public yy::parser {
  private:
    Lexer *scanner_;
    std::unique_ptr<Program> root_;
    std::vector<std::string> source_lines_;

  public:
    Error_collector error_collector;
    Scope scopes;

    My_parser(Lexer *scanner, std::unique_ptr<language::Program> &root,
              const std::string &program_file)
        : yy::parser(scanner, root, this), scanner_(scanner),
          root_(std::move(root)), error_collector(program_file) {
        read_source(program_file);
    }

    void read_source(std::string_view file_name) {
        std::ifstream input_file(std::string{file_name});
        std::string line;
        while (std::getline(input_file, line))
            source_lines_.push_back(line);
    }

    std::string_view get_line_content(const int num_line) const {
        return source_lines_[num_line - 1];
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_MY_PARSER_HPP
