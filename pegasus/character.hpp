#ifndef PEGASUS_HEADER_CHARACTER
#define PEGASUS_HEADER_CHARACTER

#include "character_range.hpp"
#include "sequence.hpp"
#include "stack_action.hpp"

namespace pegasus {
    inline constexpr auto ch(char32_t value) {
        return chr(value, value) >> act([](take<char32_t>){});
    }
}

#endif
