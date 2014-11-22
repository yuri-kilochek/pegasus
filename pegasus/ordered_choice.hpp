#ifndef PEGASUS_HEADER_PARSERS_ORDERED_CHOICE
#define PEGASUS_HEADER_PARSERS_ORDERED_CHOICE

#include <utility>

#include "failure.hpp"

namespace pegasus {
    template <typename First, typename Second>
    struct ordered_choice {
        First first;
        Second second;

        template <typename State>
        auto parse(State&& state) const {
            auto backup = state;
            try {
                return first.parse(std::move(state));
            } catch (failure const&) {
                return second.parse(std::move(backup));
            }
        }
    };

    template <typename First, typename Second>
    inline constexpr auto operator|(First&& first, Second&& second) {
        return ordered_choice<First, Second>{std::forward<First>(first), std::forward<Second>(second)};
    }
}

#endif
