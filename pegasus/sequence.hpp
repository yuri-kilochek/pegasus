#ifndef PEGASUS_HEADER_SEQUENCE
#define PEGASUS_HEADER_SEQUENCE

#include <utility>

#include "basic_parser.hpp"

namespace pegasus {
    template <typename First, typename Second>
    struct sequence
        : basic_parser
    {
        template <typename FirstArg, typename SecondArg>
        explicit constexpr sequence(FirstArg&& first_arg, SecondArg&& second_arg)
            : first(std::forward<FirstArg>(first_arg))
            , second(std::forward<SecondArg>(second_arg))
        {}

        sequence(sequence const&) = default;
        sequence(sequence&&) = default;

        template <typename State>
        auto parse(State&& state) const {
            return second.parse(first.parse(std::move(state)));
        }

    private:
        First first;
        Second second;
    };

    template <typename First, typename Second, enable_if_parsers<First, Second>...>
    inline constexpr auto operator>>(First&& first, Second&& second) {
        return sequence<First, Second>(std::forward<First>(first), std::forward<Second>(second));
    }
}

#endif
