#ifndef PEGASUS_HEADER_STACK_ACTION
#define PEGASUS_HEADER_STACK_ACTION

#include <memory>
#include <utility>
#include <type_traits>

#include "basic_parser.hpp"
#include "list.hpp"
#include "state.hpp"
#include "impl/reverse_targs.hpp"

namespace pegasus {
    namespace stack_action_impl {
        template <typename Type, typename Tag>
        struct arg {
            arg(Type& ref)
                : ref(ref)
            {}

            Type& operator*() {
                return ref;
            }

            Type* operator->() {
                return std::addressof(ref);
            }

        private:
            Type& ref;
        };

        struct put_tag;
        struct edit_tag;
        struct take_tag;
    }

    template <typename Type>
    using put = stack_action_impl::arg<Type, stack_action_impl::put_tag>;

    template <typename Type>
    using edit = stack_action_impl::arg<Type, stack_action_impl::edit_tag>;

    template <typename Type>
    using take = stack_action_impl::arg<Type, stack_action_impl::take_tag>;

    namespace stack_action_impl {
        template <typename... ReversedArgs>
        struct applicator;

        template <typename Arg, typename... RemainingArgs>
        struct applicator<put<Arg>, RemainingArgs...> {
            template <typename... Values, typename Callable, typename... UnpackedArgs>
            static auto apply(list_node_ptr<Values...> values, Callable& callable, UnpackedArgs&&... unpacked_args) {
                Arg arg;
                auto new_values = applicator<RemainingArgs...>::apply(std::move(values), callable, arg, std::forward<UnpackedArgs>(unpacked_args)...);
                return new_list_node(std::move(arg), std::move(new_values));
            }
        };

        template <typename Arg, typename... RemainingArgs>
        struct applicator<edit<Arg>, RemainingArgs...> {
            template <typename... Values, typename Callable, typename... UnpackedArgs>
            static auto apply(list_node_ptr<Values...> values, Callable& callable, UnpackedArgs&&... unpacked_args) {
                Arg arg = std::move(values->first);
                auto new_rest = applicator<RemainingArgs...>::apply(std::move(values->rest), callable, arg, std::forward<UnpackedArgs>(unpacked_args)...);
                return new_list_node(std::move(arg), std::move(new_rest));
            }
        };

        template <typename Arg, typename... RemainingArgs>
        struct applicator<take<Arg>, RemainingArgs...> {
            template <typename... Values, typename Callable, typename... UnpackedArgs>
            static auto apply(list_node_ptr<Values...> values, Callable& callable, UnpackedArgs&&... unpacked_args) {
                Arg arg = std::move(values->first);
                return applicator<RemainingArgs...>::apply(std::move(values->rest), callable, arg, std::forward<UnpackedArgs>(unpacked_args)...);
            }
        };

        template <>
        struct applicator<> {
            template <typename... Values, typename Callable, typename... UnpackedArgs>
            static auto apply(list_node_ptr<Values...> values, Callable& callable, UnpackedArgs&&... unpacked_args) {
                callable(std::forward<UnpackedArgs>(unpacked_args)...);
                return std::move(values);
            }
        };

        template <typename Callable, typename... Args>
        inline auto dispatch(void (Callable::*)(Args...) const) {
            return impl::reverse_targs<applicator, Args...>{};
        }
    }

    template <typename Callable>
    struct stack_action
        : basic_parser
    {
        template <typename CallableArg>
        explicit constexpr stack_action(CallableArg&& callable_arg)
            : callable(std::forward<CallableArg>(callable_arg))
        {}

        stack_action(stack_action const&) = default;
        stack_action(stack_action&&) = default;

        template <typename State>
        auto parse(State&& state) const {
            using callable_type = typename std::remove_reference<Callable>::type;
            using applicator = decltype(stack_action_impl::dispatch(&callable_type::operator()));
            return make_state(applicator::apply(std::move(state.values), callable), std::move(state.cursor));
        }

    private:
        Callable callable;
    };

    template <typename Callable>
    inline constexpr auto act(Callable&& callable) {
        return stack_action<Callable>(std::forward<Callable>(callable));
    }
}

#endif
