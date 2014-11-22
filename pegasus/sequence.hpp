#ifndef PEGASUS_HEADER_SEQUENCE
#define PEGASUS_HEADER_SEQUENCE

#include <utility>

namespace pegasus {
    template <typename First, typename Second>
    struct sequence {
        First first;
        Second second;

        template <typename State>
        auto parse(State&& state) const {
            return second.parse(first.parse(std::move(state)));
        }
    };

    template <typename First, typename Second>
    inline constexpr auto operator>>(First&& first, Second&& second) {
        return sequence<First, Second>{std::forward<First>(first), std::forward<Second>(second)};
    }
}

#endif
