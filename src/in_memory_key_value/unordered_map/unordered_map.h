#ifndef TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_H
#define TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_H

#include <vector>
#include <iostream>
#include <forward_list>

// ttl Transactions Template Library
namespace ttl {
    template <typename Key, typename Value, typename Hash = std::hash<Key>>
    class unordered_map {
    public:
        using value_type = std::pair<const Key, Value>;
        using key_type   = Key;

    private:
        struct node {
            value_type data;
            size_t cache;
        };

        std::forward_list<node> elements_;
        std::vector<typename std::forward_list<node>::iterator> pointers_;
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_UNORDERED_MAP_H
