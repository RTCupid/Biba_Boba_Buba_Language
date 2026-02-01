#ifndef FRONTEND_INCLUDE_SCOPE_HPP
#define FRONTEND_INCLUDE_SCOPE_HPP

#include "config.hpp"
#include <cassert>
#include <unordered_set>
#include <vector>

namespace language {

class Scope final {
  private:
    std::vector<nametable_t> scopes_;

  public:
    Scope() {
        push(nametable_t{}); // add global scope
    }

    void push(nametable_t nametable) { scopes_.push_back(nametable); }

    void pop() { scopes_.pop_back(); }

    void add_variable(const name_t &var_name) {
        assert(!scopes_.empty());
        scopes_.back().emplace(var_name);
    }

    bool find(const name_t &var_name) const {
        for (auto it = scopes_.rbegin(), last_it = scopes_.rend();
             it != last_it; ++it) {
            if (it->find(var_name) != it->end())
                return true;
        }

        return false;
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_UTILS_HPP
