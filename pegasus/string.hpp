#ifndef PEGASUS_HEADER_STRING
#define PEGASUS_HEADER_STRING

#include <cstddef>

#include "character.hpp"
#include "sequence.hpp"
#include "empty_string.hpp"

namespace pegasus {
    template <std::size_t size>
    inline constexpr auto string_impl(char32_t const* value) {
        return ch(*value) >> string_impl<size - 1>(value + 1);
    }

    template <>
    inline constexpr auto string_impl<0>(char32_t const*) {
        return estr;
    }

    template <std::size_t size>
    inline constexpr auto str(char32_t const (&value)[size]) {
        return string_impl<size - 1>(value);
    }
}

#endif
