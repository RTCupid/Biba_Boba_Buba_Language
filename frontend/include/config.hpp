#ifndef FRONTEND_INCLUDE_CONFIG_HPP
#define FRONTEND_INCLUDE_CONFIG_HPP

#include "iterative_ast_deleter.hpp"
#include <memory>
#include <string>
#include <unordered_set>

namespace language {

using number_t = int;
using name_t = std::string;

using nametable_t = std::unordered_set<name_t>;
class Program;

using program_ptr = std::unique_ptr<Program, Iterative_ast_deleter>;

} // namespace language

#endif // FRONTEND_INCLUDE_CONFIG_HPP
