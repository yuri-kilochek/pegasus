#ifndef PEGASUS_HEADER_LIST
#define PEGASUS_HEADER_LIST

#include <utility>
#include <cstddef>
#include <memory>

namespace pegasus {
    template <typename First, typename... Rest>
    struct list_node;

    template <typename... Values>
    struct list_node_ptr;

    template <>
    struct list_node_ptr<> {};

    static constexpr auto empty_list = list_node_ptr<>{};

    template <typename First, typename... Rest>
    struct list_node_ptr<First, Rest...> {
        explicit list_node_ptr(list_node<First, Rest...>* node, std::size_t* refc)
            : node(node)
            , refc(refc)
        {}

        list_node_ptr(decltype(nullptr) = nullptr)
            : node(nullptr)
        {}

        list_node_ptr(list_node_ptr const& other)
            : node(other.node)
        {
            if (node) {
                refc = other.refc;
                ++*refc;
            }
        }

        list_node_ptr(list_node_ptr&& other)
            : node(other.node)
        {
            if (node) {
                refc = other.refc;
                other.node = nullptr;
            }
        }

        list_node_ptr& operator=(list_node_ptr other) {
            swap(*this, other);
            return *this;
        }

        ~list_node_ptr() {
            if (node && --*refc == 0) {
                delete node;
                delete refc;
            }
        }

        friend void swap(list_node_ptr& a, list_node_ptr& b) {
            std::swap(a.node, b.node);
            std::swap(a.refc, b.refc);
        }

        list_node<First, Rest...>* operator->() const {
            if (node && *refc > 1) {
                std::unique_ptr<list_node<First, Rest...>> new_node(new list_node<First, Rest...>(*node));
                std::unique_ptr<std::size_t> new_refc(new std::size_t{1});
                node = new_node.release();
                refc = new_refc.release();
            }
            return node;
        }

        list_node<First, Rest...>& operator*() const {
            return *operator->();
        }

    private:
        mutable list_node<First, Rest...>* node;
        mutable std::size_t* refc;
    };

    template <typename First, typename... Rest>
    inline list_node_ptr<First, Rest...> new_list_node(First first, list_node_ptr<Rest...> rest) {
        std::unique_ptr<list_node<First, Rest...>> node(new list_node<First, Rest...>{std::move(first), std::move(rest)});
        std::unique_ptr<std::size_t> refc(new std::size_t{1});
        return list_node_ptr<First, Rest...>(node.release(), refc.release());
    }

    template <typename First, typename... Rest>
    struct list_node {
        First first;
        list_node_ptr<Rest...> rest;
    };
}

#endif
