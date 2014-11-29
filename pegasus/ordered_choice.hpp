#ifndef PEGASUS_HEADER_PARSERS_ORDERED_CHOICE
#define PEGASUS_HEADER_PARSERS_ORDERED_CHOICE

#include <utility>

#include "basic_parser.hpp"
#include "failure.hpp"

namespace pegasus {
    template <typename First, typename Second>
    struct ordered_choice
        : basic_parser
    {
        template <typename FirstArg, typename SecondArg>
        explicit constexpr ordered_choice(FirstArg&& first_arg, SecondArg&& second_arg)
            : first(std::forward<FirstArg>(first_arg))
            , second(std::forward<SecondArg>(second_arg))
        {}

        ordered_choice(ordered_choice const&) = default;
        ordered_choice(ordered_choice&&) = default;

        template <typename State>
        auto parse(State&& state) const {
            auto backup = state;
            try {
                return first.parse(std::move(state));
            } catch (failure const&) {
                return second.parse(std::move(backup));
            }
        }

    private:
        First first;
        Second second;
    };

    template <typename First, typename Second, enable_if_parsers<First, Second>...>
    inline constexpr auto operator|(First&& first, Second&& second) {
        return ordered_choice<First, Second>(std::forward<First>(first), std::forward<Second>(second));
    }
}

#endif
