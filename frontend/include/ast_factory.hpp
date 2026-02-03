#ifndef FRONTEND_INCLUDE_AST_FACTORY_HPP
#define FRONTEND_INCLUDE_AST_FACTORY_HPP

#include "config.hpp"
#include "iterative_ast_deleter.hpp"
#include "node.hpp"
#include <memory>

namespace language {

class AST_Factory final {
  public:
    static program_ptr makeProgram(StmtList stmts) {
        return program_ptr(new Program(std::move(stmts)));
    }

    template <typename T, typename... Args>
    static std::unique_ptr<T> make(Args &&...args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_FACTORY_HPP
