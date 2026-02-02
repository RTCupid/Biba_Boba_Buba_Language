#ifndef FRONTEND_INCLUDE_AST_FACTORY_HPP
#define FRONTEND_INCLUDE_AST_FACTORY_HPP

#include "node.hpp"
#include <memory>
#include "iterative_ast_deleter.hpp" 

namespace language {

class AST_Factory final {
  public:
    static std::unique_ptr<Program, Iterative_ast_deleter> makeProgram(StmtList stmts) {
        return std::unique_ptr<Program, Iterative_ast_deleter>(new Program(std::move(stmts)));
    }

    static Statement_ptr makeEmpty() { return std::make_unique<Empty_stmt>(); }

    static Statement_ptr makeBlock(StmtList stmts = {}) {
        return std::make_unique<Block_stmt>(std::move(stmts));
    }

    static Statement_ptr makeWhile(Expression_ptr condition,
                                   Statement_ptr body) {
        return std::make_unique<While_stmt>(std::move(condition),
                                            std::move(body));
    }

    static Statement_ptr makeIf(Expression_ptr condition,
                                Statement_ptr then_branch,
                                Statement_ptr else_branch = nullptr) {
        return std::make_unique<If_stmt>(std::move(condition),
                                         std::move(then_branch),
                                         std::move(else_branch));
    }

    static Expression_ptr makeAssignmentExpr(Variable_ptr variable,
                                             Expression_ptr expression) {
        return std::make_unique<Assignment_expr>(std::move(variable),
                                                 std::move(expression));
    }

    static Statement_ptr makeAssignmentStmt(Variable_ptr variable,
                                            Expression_ptr expression) {
        return std::make_unique<Assignment_stmt>(std::move(variable),
                                                 std::move(expression));
    }

    static Expression_ptr makeInput() { return std::make_unique<Input>(); }

    static Statement_ptr makePrint(Expression_ptr expression) {
        return std::make_unique<Print_stmt>(std::move(expression));
    }

    static Expression_ptr makeBinaryOp(Binary_operators op, Expression_ptr left,
                                       Expression_ptr right) {
        return std::make_unique<Binary_operator>(op, std::move(left),
                                                 std::move(right));
    }

    static Expression_ptr makeUnaryOp(Unary_operators op,
                                      Expression_ptr operand) {
        return std::make_unique<Unary_operator>(op, std::move(operand));
    }

    static Expression_ptr makeNumber(number_t number) {
        return std::make_unique<Number>(number);
    }

    static Variable_ptr makeVariable(name_t name) {
        return std::make_unique<Variable>(name);
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_AST_FACTORY_HPP
