#ifndef FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP
#define FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP

#include "parser.hpp"

namespace language {

class Error_collector {
  private:
    struct Error_info {
        location loc, std::string msg,
    };
};

} // namespace language

#endif // FRONTEND_INCLUDE_ERROR_COLLECTOR_HPP
