#ifndef PEGASUS_HEADER_NOTHING
#define PEGASUS_HEADER_NOTHING

#include <utility>

#include "failure.hpp"

namespace pegasus {
    struct nothing {
        template <typename State>
        auto parse(State&& state) const {
            throw failure{};
        }
    };

    static constexpr auto nil = nothing{};
}

#endif
