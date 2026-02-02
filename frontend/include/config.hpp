#ifndef FRONTEND_INCLUDE_CONFIG_HPP
#define FRONTEND_INCLUDE_CONFIG_HPP

#include <string>
#include <memory>
#include "iterative_ast_deleter.hpp"

namespace language {

using number_t = int;
using name_t = std::string;

class Program;

using program_ptr = std::unique_ptr<Program, Iterative_ast_deleter>;

} // namespace language

#endif // FRONTEND_INCLUDE_CONFIG_HPP
