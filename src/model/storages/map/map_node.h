#ifndef TRANSACTIONS_LIBRARY_CPP_MAP_NODE_H
#define TRANSACTIONS_LIBRARY_CPP_MAP_NODE_H

#include <iostream>
#include <memory>

namespace ttl {
    template <typename Key, typename Value>
    struct MapNode {
        using value_type = std::pair<const Key, Value>;
        using key_type = Key;
        using mapped_type = Value;

        std::pair<const Key, Value> key_value;

        bool is_red = false;

        MapNode *left   = nullptr;
        MapNode *right  = nullptr;
        MapNode *parent = nullptr;

        std::size_t count = 0;

        MapNode() noexcept = default;
        explicit MapNode(const std::pair<Key, Value> &kv) : key_value(kv) {};
        explicit MapNode(std::pair<Key, Value> &&kv) noexcept : key_value(std::move(kv)) {};
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_MAP_NODE_H
