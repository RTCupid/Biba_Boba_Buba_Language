#ifndef FRONTEND_INCLUDE_CONFIG_HPP
#define FRONTEND_INCLUDE_CONFIG_HPP

#include <string>
#include <unordered_map>

namespace language {

using number_t = int;
using name_t = std::string;
using nametable_t = std::unordered_map<language::name_t, bool /*defined*/>;

} // namespace language


#endif // FRONTEND_INCLUDE_CONFIG_HPP
