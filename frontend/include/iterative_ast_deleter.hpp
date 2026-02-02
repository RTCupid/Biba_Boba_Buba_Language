#ifndef FRONTEND_INCLUDE_ITERATIVE_AST_DELETER_HPP
#define FRONTEND_INCLUDE_ITERATIVE_AST_DELETER_HPP

namespace language {

class Program; 

struct Iterative_ast_deleter {
    void operator()(Program* root) const noexcept;
};

} // namespace language

#endif // FRONTEND_INCLUDE_ITERATIVE_AST_DELETER_HPP
