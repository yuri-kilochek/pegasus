#ifndef PEGASUS_HEADER_STATE
#define PEGASUS_HEADER_STATE

#include <type_traits>
#include <utility>

namespace pegasus {
    template <typename Values, typename Cursor>
    struct state {
        Values values;
        Cursor cursor;
    };

    template <typename Values, typename Cursor>
    inline auto make_state(Values&& values, Cursor&& cursor)
        -> state<typename std::decay<Values>::type, typename std::decay<Cursor>::type>
    {
        return {std::forward<Values>(values), std::forward<Cursor>(cursor)};
    }
}

#endif
