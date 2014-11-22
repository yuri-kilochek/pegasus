#ifndef PEGASUS_HEADER_CHARACTER_SET
#define PEGASUS_HEADER_CHARACTER_SET

#include <cstddef>

#include "character.hpp"
#include "ordered_choice.hpp"
#include "nothing.hpp"

namespace pegasus {
    template <std::size_t size>
    inline constexpr auto character_set_impl(char32_t const* value) {
        return ch(*value) | character_set_impl<size - 1>(value + 1);
    }

    template <>
    inline constexpr auto character_set_impl<0>(char32_t const*) {
        return nil;
    }

    template <std::size_t size>
    inline constexpr auto chs(char32_t const (&value)[size]) {
        return character_set_impl<size - 1>(value);
    }
}

#endif
