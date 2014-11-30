#ifndef PEGASUS_HEADER_STRING
#define PEGASUS_HEADER_STRING

#include <cstddef>

#include "sequence.hpp"
#include "character.hpp"
#include "stack_action.hpp"
#include "empty_string.hpp"

namespace pegasus {
    template <std::size_t size>
    inline constexpr auto string_impl(char32_t const* value) {
        return string_impl<size - 1>(value) >>
               ch(value[size - 1]) >>
               act([](edit<std::u32string> s, take<char32_t> c){ *s += *c; });
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
