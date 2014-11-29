#ifndef PEGASUS_HEADER_IMPL_ENABLE_IF
#define PEGASUS_HEADER_IMPL_ENABLE_IF

namespace pegasus {
    namespace impl {
        template <bool condition>
        struct enable_if_impl {};

        template <>
        struct enable_if_impl<true> {
            enum class sfinae_victim {};
        };

        template <bool condition>
        using enable_if = typename enable_if_impl<condition>::sfinae_victim;
    }
}

#endif
