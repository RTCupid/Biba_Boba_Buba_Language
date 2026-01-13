#ifndef FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP
#define FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP

#include "parser.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace language {

class Error_collector final {
  private:
    const std::string program_file_;

    struct Error_info {
        const std::string program_file_;
        const yy::location loc_;
        const std::string msg_;

        Error_info(const std::string program_file, const yy::location &loc, const std::string &msg)
            : program_file_(program_file), loc_(loc), msg_(msg) {}

        void print(std::ostream &os) const {
            os << program_file_ << ':' << loc_.begin.line << ':'
               << loc_.begin.column << ": error: " << msg_ << '\n';
        }
    };

    std::vector<Error_info> errors_;
  public:
    Error_collector(const std::string &program_file) : program_file_(program_file) {}

    void add_error(const yy::location &loc, const std::string &msg) {
        errors_.push_back(Error_info{program_file_, loc, msg});
    }

    bool has_errors() const { return !errors_.empty(); }

    void print_errors(std::ostream &os) const {
        if (!errors_.empty())
            for (auto &error : errors_) {
                error.print(os);
            }
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP
