#ifndef PEGASUS_HEADER_UTF8
#define PEGASUS_HEADER_UTF8

#include <stdexcept>
#include <iterator>
#include <cassert>
#include <cstddef>

namespace pegasus {
    struct utf8_coding_error
        : std::invalid_argument
    {
        using std::invalid_argument::invalid_argument;
    };

    template <typename InputIterator>
    struct utf8_decoding_iterator
        : std::iterator<std::input_iterator_tag, char32_t>
    {
        utf8_decoding_iterator(InputIterator const& input_iterator)
            : begin(input_iterator)
            , end(input_iterator)
        {}

        utf8_decoding_iterator(utf8_decoding_iterator const&) = default;
        utf8_decoding_iterator(utf8_decoding_iterator&&) = default;
        utf8_decoding_iterator& operator=(utf8_decoding_iterator const&) = default;
        utf8_decoding_iterator& operator=(utf8_decoding_iterator&&) = default;

        friend bool operator==(utf8_decoding_iterator const& a, utf8_decoding_iterator const& b) {
            return a.begin == b.begin;
        }

        friend bool operator!=(utf8_decoding_iterator const& a, utf8_decoding_iterator const& b) {
            return !(a == b);
        }

        reference operator*() const {
            ensure_consumed();
            return value;
        }

        pointer operator->() const {
            ensure_consumed();
            return &value;
        }

        utf8_decoding_iterator& operator++() {
            ensure_consumed();
            begin = end;
            value = -1;
            return *this;
        }

        struct post_incremented_proxy {
            post_incremented_proxy(utf8_decoding_iterator& decoder)
                : decoder(decoder)
            {}

            ~post_incremented_proxy() {
                ++decoder;
            }

            reference operator*() {
                return *decoder;
            }

        private:
            utf8_decoding_iterator& decoder;
        };

        post_incremented_proxy operator++(int) {
            return {*this};
        }

    private:
        InputIterator begin;
        mutable InputIterator end;
        mutable value_type value = -1;

        void ensure_consumed() const {
            if (value == -1) {
                std::size_t trail_size;

                if (((unsigned char)*end & 0b10000000) == 0b00000000) {
                    value = (unsigned char)*end++ & 0b01111111;
                    trail_size = 0;
                } else if (((unsigned char)*end & 0b11100000) == 0b11000000) {
                    value = (unsigned char)*end++ & 0b00011111;
                    trail_size = 1;
                } else if (((unsigned char)*end & 0b11110000) == 0b11100000) {
                    value = (unsigned char)*end++ & 0b00001111;
                    trail_size = 2;
                } else if (((unsigned char)*end & 0b11111000) == 0b11110000) {
                    value = (unsigned char)*end++ & 0b00000111;
                    trail_size = 3;
                } else {
                    throw utf8_coding_error("Illegal utf8");
                }

                while (trail_size-- > 0) {
                    if (((unsigned char)*end & 0b11000000) != 0b10000000) {
                        throw utf8_coding_error("Illegal utf8");
                    }
                    value = (value << 6) | (unsigned char)*end++ & 0b00111111;
                }
            }
        }
    };

    template <typename InputIterator>
    utf8_decoding_iterator<InputIterator> utf8_decoder(InputIterator const& input_iterator) {
        return {input_iterator};
    }

    template <typename OutputIterator>
    struct utf8_encoding_iterator
        : std::iterator<std::output_iterator_tag, void, void, void, void>
    {
        utf8_encoding_iterator(OutputIterator const& output_iterator)
            : it(output_iterator)
            , value(-1)
        {}

        utf8_encoding_iterator(utf8_encoding_iterator const&) = default;
        utf8_encoding_iterator(utf8_encoding_iterator&&) = default;
        utf8_encoding_iterator& operator=(utf8_encoding_iterator const&) = default;
        utf8_encoding_iterator& operator=(utf8_encoding_iterator&&) = default;

        ~utf8_encoding_iterator() {
            if (value != -1) {
                emit();
            }
        }

        char32_t& operator*() const {
            return value;
        }

        utf8_encoding_iterator& operator++() {
            emit();
            return *this;
        }

        struct post_incremented_proxy {
            post_incremented_proxy(utf8_encoding_iterator& decoder)
                : encoder(encoder)
            {}

            ~post_incremented_proxy() {
                ++encoder;
            }

            reference operator*() {
                return *encoder;
            }

        private:
            utf8_encoding_iterator& encoder;
        };

        post_incremented_proxy operator++(int) {
            return {*this};
        }

    private:
        OutputIterator it;
        mutable char32_t value;

        void emit() {
            assert(value != -1);

            std::size_t trail_size;

            if (value < 0x80) {
                *it++ = 0b00000000 | (value >> 0) & 0b01111111;
                trail_size = 0;
            } else if (value < 0x800) {
                *it++ = 0b11000000 | (value >> 6) & 0b00011111;
                trail_size = 1;
            } else if (value < 0x10000) {
                *it++ = 0b11100000 | (value >> 12) & 0b00001111;
                trail_size = 2;
            } else if (value < 0x200000) {
                *it++ = 0b11110000 | (value >> 18) & 0b00000111;
                trail_size = 3;
            } else {
                throw utf8_coding_error("Illegal code point");
            }

            while (trail_size-- > 0) {
                *it++ = 0b10000000 | (value >> trail_size * 6) & 0b00111111;
            }

            value = -1;
        }
    };

    template <typename InputIterator>
    utf8_encoding_iterator<InputIterator> utf8_encoder(InputIterator const& input_iterator) {
        return {input_iterator};
    }
}

#endif
