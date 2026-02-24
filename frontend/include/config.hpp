#ifndef FRONTEND_INCLUDE_CONFIG_HPP
#define FRONTEND_INCLUDE_CONFIG_HPP

#include <string>
#include <string_view>
#include <unordered_set>

namespace language {

using number_t = int;

using name_t_sv = std::string_view;
using name_t = std::string;

using nametable_t = std::unordered_set<name_t>;

class Program;

using program_ptr = Program *;

} // namespace language

#endif // FRONTEND_INCLUDE_CONFIG_HPP
