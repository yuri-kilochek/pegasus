#ifndef PEGASUS_HEADER_CHARACTER_RANGE
#define PEGASUS_HEADER_CHARACTER_RANGE

#include <utility>

#include "basic_parser.hpp"
#include "state.hpp"
#include "failure.hpp"

namespace pegasus {
    struct character_range
        : basic_parser
    {
        explicit constexpr character_range(char32_t first, char32_t last)
            : first(first)
            , last(last)
        {}

        character_range(character_range const&) = default;
        character_range(character_range&&) = default;

        template <typename State>
        auto parse(State&& state) const {
            if (state.cursor) {
                char32_t value = *state.cursor;
                if (first <= value && value <= last) {
                    return make_state(new_list_node(value, std::move(state.values)), std::move(state.cursor).next());
                }
            }
            throw failure{};
        }

    private:
        char32_t first;
        char32_t last;
    };

    inline constexpr auto chr(char32_t first, char32_t last) {
        return character_range(first, last);
    }
}

#endif
