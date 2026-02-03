#ifndef FRONTEND_INCLUDE_AST_FACTORY_HPP
#define FRONTEND_INCLUDE_AST_FACTORY_HPP

#include "iterative_ast_deleter.hpp"
#include <memory>

namespace language {

class AST_Factory final {
  public:
    template <typename T, typename... Args>
    static std::unique_ptr<T> make(Args &&...args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_FACTORY_HPP
