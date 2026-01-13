#ifndef FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP
#define FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP

#include "parser.hpp"
#include <ostream>
#include <string>
#include <vector>

namespace language {

class Error_collector {
  private:
    struct Error_info {
        const yy::location loc_;
        const std::string msg_;

        Error_info(const yy::location &loc, const std::string &msg)
            : loc_(loc), msg_(msg) {}

        void print(std::ostream &os) const {
            os << "Syntax error at line " << loc_.begin.line << ", column "
               << loc_.begin.column << ": " << msg_ << '\n';
        }
    };

    std::vector<Error_info> errors_;

  public:
    void add_error(const yy::location &loc, const std::string &msg) {
        errors_.push_back(Error_info{loc, msg});
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
