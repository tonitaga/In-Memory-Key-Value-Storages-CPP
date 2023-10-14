#ifndef TRANSACTIONS_LIBRARY_CPP_MAP_NODE_H
#define TRANSACTIONS_LIBRARY_CPP_MAP_NODE_H

#include <iostream>
#include <memory>

namespace ttl {
    template <typename Key, typename Value>
    struct MapNode {
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<key_type, mapped_type>;
        using size_type = std::size_t;

        std::pair<key_type, mapped_type> key_value;

        bool is_red = false;

        MapNode *left   = nullptr;
        MapNode *right  = nullptr;
        MapNode *parent = nullptr;

        std::size_t count = 0;

        MapNode() noexcept = default;

        explicit MapNode(const value_type &kv) : key_value(kv) {};
        explicit MapNode(value_type &&kv) noexcept : key_value(std::move(kv)) {};
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_MAP_NODE_H
