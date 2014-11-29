#ifndef PEGASUS_HEADER_REPEATED
#define PEGASUS_HEADER_REPEATED

#include <utility>

#include "basic_parser.hpp"
#include "failure.hpp"

namespace pegasus {
    template <typename Parser>
    struct repeated
        : basic_parser
    {
        template <typename ParserArg>
        explicit constexpr repeated(ParserArg&& parser_arg)
            : parser(std::forward<ParserArg>(parser_arg))
        {}

        repeated(repeated const&) = default;
        repeated(repeated&&) = default;

        template <typename State>
        auto parse(State&& state) const {
            while (true) {
                auto backup = state;
                try {
                    state = parser.parse(std::move(state));
                } catch (failure const&) {
                    return std::move(backup);
                }
            }
        }

    private:
        Parser parser;
    };

    template <typename Parser, enable_if_parsers<Parser>...>
    inline constexpr auto operator*(Parser&& parser) {
        return repeated<Parser>(std::forward<Parser>(parser));
    }
}

#endif
