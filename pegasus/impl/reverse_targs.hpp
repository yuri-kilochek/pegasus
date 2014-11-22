#ifndef PEGASUS_HEADER_IMPL_REVERSE_TARGS
#define PEGASUS_HEADER_IMPL_REVERSE_TARGS

#include <tuple>

namespace pegasus {
    namespace impl {
        template <template <typename...> class Template, typename Types, typename ReversedTypes>
        struct reverse_targs_impl;

        template <template <typename...> class Template, typename First, typename... Rest, typename... ReversedTypes>
        struct reverse_targs_impl<Template, std::tuple<First, Rest...>, std::tuple<ReversedTypes...>>
            : reverse_targs_impl<Template, std::tuple<Rest...>, std::tuple<First, ReversedTypes...>>
        {};

        template <template <typename...> class Template, typename... ReversedTypes>
        struct reverse_targs_impl<Template, std::tuple<>, std::tuple<ReversedTypes...>> {
            using type = Template<ReversedTypes...>;
        };

        template <template <typename...> class Template, typename... Types>
        using reverse_targs = typename reverse_targs_impl<Template, std::tuple<Types...>, std::tuple<>>::type;
    }
}

#endif
