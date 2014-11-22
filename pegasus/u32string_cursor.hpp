#ifndef PEGASUS_HEADER_U32STRING_CURSOR
#define PEGASUS_HEADER_U32STRING_CURSOR

#include <string>
#include <iterator>

#include "parse.hpp"

namespace pegasus {
    struct u32string_cursor {
        explicit u32string_cursor(std::u32string const& string, std::u32string::const_iterator const& position)
            : string(&string)
            , position(position)
        {}

        u32string_cursor(u32string_cursor const& other) = default;
        u32string_cursor(u32string_cursor&& other) = default;
        u32string_cursor& operator=(u32string_cursor const& other) = default;
        u32string_cursor& operator=(u32string_cursor&& other) = default;

        explicit operator bool() const {
            return position != string->end();
        }

        char32_t operator*() const {
            return *position;
        }

        u32string_cursor next() const {
            return u32string_cursor(*string, std::next(position));
        }

    private:
        std::u32string const* string;
        std::u32string::const_iterator position;
    };

    template <typename Parser>
    auto parse(Parser const& parser, std::u32string const& string) {
        return parse(parser, u32string_cursor(string, string.begin()));
    }

    template <typename Parser>
    auto parse(Parser const& parser, char32_t const* string) {
        return parse(parser, std::u32string(string));
    }
}

#endif
