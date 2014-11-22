#ifndef PEGASUS_HEADER_OPTIONAL
#define PEGASUS_HEADER_OPTIONAL

#include "ordered_choice.hpp"
#include "empty_string.hpp"

namespace pegasus {
    template <typename Parser>
    inline constexpr auto operator~(Parser&& parser) {
        return static_cast<Parser>(parser) | estr;
    }
}

#endif
