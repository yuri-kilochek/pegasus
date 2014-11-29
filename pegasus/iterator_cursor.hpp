#ifndef PEGASUS_HEADER_CHAR32_CURSOR
#define PEGASUS_HEADER_CHAR32_CURSOR

#include <string>
#include <iterator>

#include "parse.hpp"

namespace pegasus {
    template <typename Iterator>
    struct iterator_cursor {
        explicit iterator_cursor(Iterator const& begin, Iterator const& end)
            : begin(begin)
            , end(end)
        {}

        iterator_cursor(iterator_cursor const& other) = default;
        iterator_cursor(iterator_cursor&& other) = default;
        iterator_cursor& operator=(iterator_cursor const& other) = default;
        iterator_cursor& operator=(iterator_cursor&& other) = default;

        explicit operator bool() const {
            return begin != end;
        }

        char32_t operator*() const {
            return *begin;
        }

        iterator_cursor next() const {
            return iterator_cursor(std::next(begin), end);
        }

    private:
        Iterator begin;
        Iterator end;
    };

    template <typename Parser, typename Iterator>
    inline auto parse(Parser const& parser, Iterator const& begin, Iterator const& end) {
        return parse(parser, iterator_cursor<Iterator>(begin, end));
    }

    template <typename Parser>
    inline auto parse(Parser const& parser, std::u32string const& string) {
        return parse(parser, string.begin(), string.end());
    }

    template <typename Parser>
    inline auto parse(Parser const& parser, char32_t const* string) {
        auto begin = string;
        auto end = begin;
        while (*end != U'\0') {
            ++end;
        }
        return parse(parser, begin, end);
    }
}

#endif
