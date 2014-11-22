#ifndef PEGASUS_HEADER_EMPTY_STRING
#define PEGASUS_HEADER_EMPTY_STRING

#include <utility>

namespace pegasus {
    struct empty_string {
        template <typename State>
        auto parse(State&& state) const {
            return std::move(state);
        }
    };

    static constexpr auto estr = empty_string{};
}

#endif
