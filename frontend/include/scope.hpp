#ifndef FRONTEND_INCLUDE_SCOPE_HPP
#define FRONTEND_INCLUDE_SCOPE_HPP

#include "config.hpp"
#include <stdexcept>
#include <unordered_set>
#include <vector>

namespace language {

class Scope final {
  private:
    std::vector<nametable_t> scopes_;

  public:
    Scope() {
        push(); // add global scope
    }

    void push(nametable_t nametable = {}) {
        scopes_.emplace_back(std::move(nametable));
    }

    void pop() {
        if (scopes_.empty()) {
            throw std::underflow_error("Scope stack is empty");
        }
        scopes_.pop_back();
    }

    void add_variable(const name_t &var_name) {
        if (scopes_.empty()) {
            throw std::underflow_error("Scope stack is empty");
        }
        scopes_.back().emplace(var_name);
    }

    bool find(const name_t &var_name) const {
        for (auto it = scopes_.rbegin(), ite = scopes_.rend(); it != ite; ++it) {
            if (it->contains(var_name)) {
                return true;
            }
        }

        return false;
    }
};

} // namespace language

#endif // FRONTEND_INCLUDE_UTILS_HPP
