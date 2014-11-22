#ifndef PEGASUS_HEADER_NOT_PREDICATE
#define PEGASUS_HEADER_NOT_PREDICATE

#include <utility>

#include "failure.hpp"

namespace pegasus {
    template <typename Parser>
    struct not_predicate {
        Parser parser;

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
    };

    template <typename Parser>
    inline constexpr auto operator!(Parser&& parser) {
        return not_predicate<Parser>{std::forward<Parser>(parser)};
    }
}

#endif
