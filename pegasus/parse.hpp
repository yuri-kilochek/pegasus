#ifndef PEGASUS_HEADER_PARSE
#define PEGASUS_HEADER_PARSE

namespace pegasus {
    namespace parse_impl {
        void get_result(list_node_ptr<>) {}

        template <typename Value>
        Value get_result(list_node_ptr<Value> values) {
            return std::move(values->first);
        }
    }

    template <typename Parser, typename Cursor>
    auto parse(Parser const& parser, Cursor cursor) {
        auto start_state = make_state(empty_list, std::move(cursor));
        auto end_state = parser.parse(std::move(start_state));
        return parse_impl::get_result(std::move(end_state.values));
    }
}

#endif
