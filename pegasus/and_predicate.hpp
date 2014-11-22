#ifndef PEGASUS_HEADER_AND_PREDICATE
#define PEGASUS_HEADER_AND_PREDICATE

#include "not_predicate.hpp"

namespace pegasus {
    template <typename Parser>
    inline constexpr auto operator&(Parser&& parser) {
        return !!static_cast<Parser>(parser);
    }
}

#endif
