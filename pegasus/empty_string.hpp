#ifndef PEGASUS_HEADER_EMPTY_STRING
#define PEGASUS_HEADER_EMPTY_STRING

#include <string>

#include "stack_action.hpp"

namespace pegasus {
    static constexpr auto estr = act([](put<std::u32string> s){ *s = U""; });
}

#endif
