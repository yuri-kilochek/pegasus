#ifndef PEGASUS_HEADER_EMPTY_STRING
#define PEGASUS_HEADER_EMPTY_STRING

#include <utility>

#include "basic_parser.hpp"

namespace pegasus {
    struct empty_string
        : basic_parser
    {
        template <typename State>
        auto parse(State&& state) const {
            return std::move(state);
        }
    };

    static constexpr auto estr = empty_string();
}

#endif
