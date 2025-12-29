#ifndef FRONTEND_INCLUDE_SCOPE_HPP
#define FRONTEND_INCLUDE_SCOPE_HPP

#include "config.hpp"
#include <unordered_map>
#include <cassert>
#include <vector>

namespace language {

using nametable_t = std::unordered_map<language::name_t, bool /*defined*/>;

class Scope {
  private:
    std::vector<nametable_t> scopes_;
public:
    Scope() {
        push(nametable_t{}); // add global scope
    }

    void push(nametable_t nametable) {
        scopes_.push_back(nametable);
    }

    void pop() { scopes_.pop_back(); }

    void add_variable(name_t &var_name, bool defined) {
        assert(!scopes_.empty());
        scopes_.back().emplace(var_name, defined);
    }

    bool find(name_t var_name) {
        for (auto it = scopes_.rbegin(), last_it = scopes_.rend();
             it != last_it; ++it) {
            auto var_iter = it->find(var_name);
            if (var_iter != it->end())
                return true;
        }

        return false;
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_UTILS_HPP
