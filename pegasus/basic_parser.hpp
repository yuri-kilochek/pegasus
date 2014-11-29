#ifndef PEGASUS_HEADER_BASIC_PARSER
#define PEGASUS_HEADER_BASIC_PARSER

#include <type_traits>

#include "impl/enable_if.hpp"

namespace pegasus {
    struct basic_parser {};

    template <typename... MaybeParsers>
    struct are_parsers;

    template <>
    struct are_parsers<>
        : std::true_type
    {};

    template <typename FirstMaybeParser, typename... RestMaybeParsers>
    struct are_parsers<FirstMaybeParser, RestMaybeParsers...>
        : std::integral_constant<
            bool,
            std::is_base_of<
                basic_parser,
                typename std::remove_cv<
                        typename std::remove_reference<FirstMaybeParser>::type
                >::type
            >::value && are_parsers<RestMaybeParsers...>::value
        >
    {};

    template <typename... MaybeParsers>
    using enable_if_parsers = impl::enable_if<are_parsers<MaybeParsers...>::value>;
}

#endif
