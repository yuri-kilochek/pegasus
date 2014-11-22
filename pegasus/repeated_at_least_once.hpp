#ifndef PEGASUS_HEADER_REPEATED_AT_LEAST_ONCE
#define PEGASUS_HEADER_REPEATED_AT_LEAST_ONCE

#include "sequence.hpp"
#include "repeated.hpp"

namespace pegasus {
    template <typename Parser>
    inline constexpr auto operator+(Parser&& parser) {
        return static_cast<Parser>(parser) >> *static_cast<Parser>(parser);
    }
}

#endif
