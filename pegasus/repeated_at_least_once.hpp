#ifndef PEGASUS_HEADER_REPEATED_AT_LEAST_ONCE
#define PEGASUS_HEADER_REPEATED_AT_LEAST_ONCE

#include "basic_parser.hpp"
#include "sequence.hpp"
#include "repeated.hpp"

namespace pegasus {
    template <typename Parser, enable_if_parsers<Parser>...>
    inline constexpr auto operator+(Parser&& parser) {
        return static_cast<Parser>(parser) >> *static_cast<Parser>(parser);
    }
}

#endif
