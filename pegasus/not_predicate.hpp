#ifndef PEGASUS_HEADER_NOT_PREDICATE
#define PEGASUS_HEADER_NOT_PREDICATE

#include <utility>

#include "basic_parser.hpp"
#include "failure.hpp"

namespace pegasus {
    template <typename Parser>
    struct not_predicate
        : basic_parser
    {
        template <typename ParserArg>
        explicit constexpr not_predicate(ParserArg&& parser_arg)
            : parser(std::forward<ParserArg>(parser_arg))
        {}

        not_predicate(not_predicate const&) = default;
        not_predicate(not_predicate&&) = default;

        template <typename State>
        auto parse(State&& state) const {
            try {
                auto dummy = state;
                parser.parse(std::move(dummy));
            } catch (failure const&) {
                return std::move(state);
            }
            throw failure{};
        }

    private:
        Parser parser;
    };

    template <typename Parser, enable_if_parsers<Parser>...>
    inline constexpr auto operator!(Parser&& parser) {
        return not_predicate<Parser>(std::forward<Parser>(parser));
    }
}

#endif
