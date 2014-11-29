#ifndef PEGASUS_HEADER_END_OF_TEXT
#define PEGASUS_HEADER_END_OF_TEXT

#include <utility>

#include "basic_parser.hpp"
#include "failure.hpp"

namespace pegasus {
    struct end_of_text
        : basic_parser
    {
        template <typename State>
        auto parse(State&& state) const {
            if (!state.cursor) {
                return std::move(state);
            }
            throw failure{};
        }
    };

    static constexpr auto eot = end_of_text();
}

#endif
