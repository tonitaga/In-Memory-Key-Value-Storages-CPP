#ifndef TRANSACTIONS_LIBRARY_CPP_HASH_TABLE_H
#define TRANSACTIONS_LIBRARY_CPP_HASH_TABLE_H

#include <iostream>
#include <vector>
#include <forward_list>

#include "unordered_map_normal_iterator.h"
#include "unordered_map_size.h"

namespace ttl {
    template <typename Key, typename Value, typename Hash = std::hash<Key>>
    class unordered_map {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<const Key, Value>;
        using hash_type = Hash;
        using size_type = std::size_t;

        using iterator = unordered_map_normal_iterator<
                typename std::vector<std::forward_list<value_type>>::iterator,
                typename std::forward_list<value_type>::iterator>;
        using const_iterator = unordered_map_normal_iterator<
                typename std::vector<std::forward_list<value_type>>::const_iterator,
                typename std::forward_list<value_type>::const_iterator>;


    private:
        using map_table_size = detail::unordered_map_size;
        using map_type = std::vector<std::forward_list<value_type>>;

    public:
        unordered_map() noexcept = default;

        std::pair<iterator, bool> insert(const std::pair<key_type, mapped_type> &kv) {
            if (map_.empty())
                resize();

            size_type hashed_key = hash_(kv.first);
            size_type hashed_key_mod = hashed_key % map_table_size::size(size_index_);

            auto &bucket = map_[hashed_key_mod];
            auto  table_it = map_.begin() + hashed_key_mod;

            for (auto b_it = bucket.begin(), b_end = bucket.end(); b_it != b_end; ++b_it)
                if (b_it->first == kv.first)
                    return std::make_pair(iterator(table_it, b_it), false);

            size_++;
            update_alpha();

            hashed_key_mod = hashed_key % map_table_size::size(size_index_);
            map_[hashed_key_mod].emplace_front(kv);

            return std::make_pair(iterator(map_.begin() + hashed_key_mod, map_[hashed_key_mod].begin()), true);
        }

        std::pair<iterator, bool> insert(std::pair<key_type, mapped_type> &&kv) {
            if (map_.empty())
                resize();

            size_type hashed_key = hash_(kv.first);
            size_type hashed_key_mod = hashed_key % map_table_size::size(size_index_);

            auto &bucket = map_[hashed_key_mod];
            auto  table_it = map_.begin() + hashed_key_mod;

            for (auto b_it = bucket.begin(), b_end = bucket.end(); b_it != b_end; ++b_it)
                if (b_it->first == kv.first)
                    return std::make_pair(iterator(table_it, b_it), false);

            size_++;
            update_alpha();

            hashed_key_mod = hashed_key % map_table_size::size(size_index_);
            map_[hashed_key_mod].emplace_front(std::move(kv));

            return std::make_pair(iterator(map_.begin() + hashed_key_mod, map_[hashed_key_mod].begin()), true);
        }

        mapped_type &operator[](const key_type &key) {
            auto it = insert(std::make_pair(key, mapped_type{})).first;
            return it->second;
        }

        mapped_type &operator[](key_type &&key) {
            auto it = insert(std::make_pair(std::move(key), mapped_type{})).first;
            return it->second;
        }

    public:

        iterator begin() noexcept { return iterator(map_.begin()); }
        const_iterator begin() const noexcept { return const_iterator(map_.begin()); }

        iterator end() noexcept { return iterator(map_.end()); }
        const_iterator end() const noexcept { return const_iterator(map_.end()); }

    public:
        [[nodiscard]] size_type size() const noexcept { return size_; }
        [[nodiscard]] size_type capacity() const noexcept { return map_table_size::size(size_index_); }

        [[nodiscard]] bool empty() const noexcept { return size_ == size_type{}; }

    public:
        iterator find(const key_type &key) {
            size_type hashed_key_mod = hash_(key) % map_table_size::size(size_index_);

            auto &bucket = map_[hashed_key_mod];
            auto table_it = map_.begin() + hashed_key_mod;

            for (auto b_it = bucket.begin(), b_end = bucket.end(); b_it != b_end; ++b_it)
                if (b_it->first == key)
                    return iterator(table_it, b_it);

            return end();
        }

        iterator find(key_type &&key) {
            size_type hashed_key_mod = hash_(std::move(key)) % map_table_size::size(size_index_);

            auto &bucket = map_[hashed_key_mod];
            auto table_it = map_.begin() + hashed_key_mod;

            for (auto b_it = bucket.begin(), b_end = bucket.end(); b_it != b_end; ++b_it)
                if (b_it->first == std::move(key))
                    return iterator(table_it, b_it);

            return end();
        }

    public:
        bool erase(const key_type &key) {
            size_type hashed_key_mod = hash_(key) % map_table_size::size(size_index_);

            auto &bucket = map_[hashed_key_mod];
            if (bucket.empty())
                return false;

            auto prev_b_it = bucket.before_begin();
            for (auto b_it = bucket.begin(), b_end = bucket.end(); b_it != b_end; ++b_it) {
                if (b_it->first == key) {
                    bucket.erase_after(prev_b_it);
                    size_--;
                    return true;
                }
                ++prev_b_it;
            }

            return false;
        }

        bool erase(key_type &&key) {
            size_type hashed_key_mod = hash_(std::move(key)) % map_table_size::size(size_index_);

            auto &bucket = map_[hashed_key_mod];
            if (bucket.empty())
                return false;

            auto prev_b_it = bucket.before_begin();
            for (auto b_it = bucket.begin(), b_end = bucket.end(); b_it != b_end; ++b_it) {
                if (b_it->first == std::move(key)) {
                    bucket.erase_after(prev_b_it);
                    size_--;
                    return true;
                }
                ++prev_b_it;
            }

            return false;
        }

        bool erase(iterator it) { return erase(it->first); }

    private:
        size_type size_index_ = 0;
        size_type size_ = 0;

        map_type map_;
        hash_type hash_;

        [[nodiscard]] double get_alpha() const { return size_ / static_cast<double>(map_table_size::size(size_index_)); }
        [[nodiscard]] double get_resize_alpha() const { return 0.75; }

        void resize() {
            size_type new_map_size = map_table_size::size(++size_index_);
            map_type new_map(new_map_size);

            for (auto &&bucket : map_) {
                if (bucket.empty())
                    continue;

                size_type key_hash = hash_(std::move(bucket.front().first)) % new_map_size;
                new_map[key_hash] = std::move(bucket);
            }

            map_ = std::move(new_map);
        }

        void update_alpha() noexcept {
            if (get_alpha() >= get_resize_alpha())
                resize();
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_HASH_TABLE_H
