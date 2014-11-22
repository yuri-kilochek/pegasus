#ifndef PEGASUS_HEADER_CHARACTER_RANGE
#define PEGASUS_HEADER_CHARACTER_RANGE

#include <utility>

#include "state.hpp"
#include "failure.hpp"

namespace pegasus {
    struct character_range {
        char32_t first;
        char32_t last;

        template <typename State>
        auto parse(State&& state) const {
            if (state.cursor) {
                char32_t value = *const_cast<decltype(state.cursor) const&>(state.cursor);
                if (first <= value && value <= last) {
                    return make_state(new_list_node(value, std::move(state.values)), std::move(state.cursor).next());
                }
            }
            throw failure{};
        }
    };

    inline constexpr auto chr(char32_t first, char32_t last) {
        return character_range{first, last};
    }
}

#endif
