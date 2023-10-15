#ifndef TRANSACTIONS_LIBRARY_CPP_map_NODE_H
#define TRANSACTIONS_LIBRARY_CPP_map_NODE_H

#include <iostream>
#include <memory>

namespace ttl::detail {
    enum class map_node_color : bool {
        kRed,
        kBlack
    };

    template <typename Key, typename Value>
    struct map_node {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<key_type, mapped_type>;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using size_type = std::size_t;

        std::pair<key_type, mapped_type> kv;

        map_node_color color = map_node_color::kBlack;

        map_node *left   = nullptr;
        map_node *right  = nullptr;
        map_node *parent = nullptr;

    public:
        map_node() noexcept = default;
        explicit map_node(const value_type &kv) : kv(kv) {};
        explicit map_node(value_type &&kv) noexcept : kv(std::move(kv)) {};

    public:
        [[nodiscard]] bool is_left_child() const { return parent and this == parent->left; }
        [[nodiscard]] bool is_right_child() const { return parent and this == parent->right; }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_map_NODE_H
