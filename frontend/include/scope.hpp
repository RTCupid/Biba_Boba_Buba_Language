#ifndef FRONTEND_INCLUDE_SCOPE_HPP
#define FRONTEND_INCLUDE_SCOPE_HPP

#include "config.hpp"
#include <vector>
#include <unordered_map>

namespace language {

class Scope {
private:
    std::vector<nametable_t> scopes_;
public:
    void push(nametable_t nametable) {
        scopes_.push_back(nametable);
    }

    void pop() {
        scopes_.pop_back();
    }

    void add_variable(name_t &var_name, bool defined) {
        scopes_.back().emplace(var_name, defined);
    }

    bool find(name_t var_name) {
        for (auto it = scopes_.rbegin(), last_it = scopes_.rend(); it != last_it; ++it) {
            auto var_iter = it->find(var_name);
            if (var_iter != it->end())
                return true;
        }

        return false;
    }

};

} // namespace language

#endif // FRONTEND_INCLUDE_UTILS_HPP
