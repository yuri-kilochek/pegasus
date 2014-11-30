#ifndef PEGASUS_HEADER_OPTIONAL
#define PEGASUS_HEADER_OPTIONAL

#include "basic_parser.hpp"
#include "ordered_choice.hpp"
#include "stack_action.hpp"

namespace pegasus {
    template <typename Parser, enable_if_parsers<Parser>...>
    inline constexpr auto operator~(Parser&& parser) {
        return static_cast<Parser>(parser) | act([](){});
    }
}

#endif
