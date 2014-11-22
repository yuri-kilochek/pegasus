#ifndef PEGASUS_HEADER_REPEATED
#define PEGASUS_HEADER_REPEATED

#include <utility>

#include "failure.hpp"

namespace pegasus {
    template <typename Parser>
    struct repeated {
        Parser parser;

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
    };

    template <typename Parser>
    inline constexpr auto operator*(Parser&& parser) {
        return repeated<Parser>{std::forward<Parser>(parser)};
    }
}

#endif
