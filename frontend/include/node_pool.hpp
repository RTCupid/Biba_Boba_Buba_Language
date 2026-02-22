// #ifndef FRONTEND_INCLUDE_AST_FACTORY_HPP
// #define FRONTEND_INCLUDE_AST_FACTORY_HPP

// #include "config.hpp"
// #include "iterative_ast_deleter.hpp"
// #include "node.hpp"
// #include <memory>
// #include <vector>

// namespace language {

// class AST_Factory final {
//   public:
//     static program_ptr makeProgram(StmtList stmts) {
//         return program_ptr(new Program(std::move(stmts)));
//     }

//     template <typename T, typename... Args>
//     static std::unique_ptr<T> make(Args &&...args) {
//         return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
//     }
// };

// } // namespace language

// #endif // FRONTEND_INCLUDE_AST_FACTORY_HPP

#ifndef FRONTEND_INCLUDE_AST_FACTORY_HPP
#define FRONTEND_INCLUDE_AST_FACTORY_HPP

#include "config.hpp"
#include "node.hpp"
#include <memory>
#include <vector>

namespace language {

class Node_pool final {
  public:
    template <typename T, typename... Args>
    T* make(Args &&... args) {
      data_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
      return static_cast<T*>(data_.back().get());
    }

  private:
    std::vector<std::unique_ptr<Node>> data_;
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_FACTORY_HPP
