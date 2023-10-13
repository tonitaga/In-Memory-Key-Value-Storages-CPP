#ifndef TRANSACTIONS_LIBRARY_CPP_HASH_TABLE_H
#define TRANSACTIONS_LIBRARY_CPP_HASH_TABLE_H

#include <iostream>
#include <vector>
#include <forward_list>

#include "unordered_map_normal_iterator.h"

namespace ttl {
    template <typename Key, typename Value, typename Hash = std::hash<Key>>
    class UnorderedMap {
    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<const Key, Value>;
        using hash_type = Hash;

        using iterator = UnorderedMapNormalIterator<
                typename std::vector<std::forward_list<value_type>>::iterator,
                typename std::forward_list<value_type>::iterator>;
        using const_iterator = UnorderedMapNormalIterator<
                typename std::vector<std::forward_list<value_type>>::const_iterator,
                typename std::forward_list<value_type>::const_iterator>;

        using size_type = std::size_t;

    public:
        UnorderedMap() : map_(map_size_) {};

        mapped_type &operator[](const key_type &key) {
            if (((size_ + 1) / static_cast<double>(map_size_)) > resize_alpha_)
                resize();

            size_type key_hash = hash_(key) % map_size_;

            auto &bucket = map_[key_hash];
            for (auto &kv : bucket) {
                if (kv.first == key)
                    return kv.second;
            }

            bucket.emplace_front(key, mapped_type());
            size_++;
            update_alpha();
            return bucket.front().second;
        }

        mapped_type &operator[](key_type &&key) {
            if (((size_ + 1) / static_cast<double>(map_size_)) > resize_alpha_)
                resize();

            size_type key_hash = hash_(std::move(key)) % map_size_;

            auto &bucket = map_[key_hash];
            for (auto &kv : bucket) {
                if (kv.first == key)
                    return kv.second;
            }

            bucket.emplace_front(std::move(key), mapped_type());
            size_++;
            update_alpha();
            return bucket.front().second;
        }

        bool insert(const std::pair<key_type, mapped_type> &kv) {
            size_type key_hash = hash_(kv.first) % map_size_;

            auto &bucket = map_[key_hash];
            for (const auto &[key, value] : bucket)
                if (key == kv.first)
                    return false;

            bucket.emplace_front(kv);
            size_++;
            update_alpha();
            return true;
        }

        bool insert(std::pair<key_type, mapped_type> &&kv) {
            size_type key_hash = hash_(std::move(kv.first)) % map_size_;

            auto &bucket = map_[key_hash];
            for (const auto &[key, value] : bucket)
                if (key == std::move(kv.first))
                    return false;

            bucket.emplace_front(std::move(kv));
            size_++;
            update_alpha();
            return true;
        }

        [[nodiscard]] bool empty() const noexcept { return size_ == size_type{}; };

        iterator find(const key_type &key) {
            size_type key_hash = hash_(key) % map_size_;

            auto &bucket = map_[key_hash];
            auto bucket_it = map_.begin() + key_hash;

            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key)
                    return iterator(bucket_it, it);
            }

            return end();
        }

        iterator find(key_type &&key) {
            size_type key_hash = hash_(std::move(key)) % map_size_;

            auto &bucket = map_[key_hash];
            auto bucket_it = map_.begin() + key_hash;

            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key)
                    return iterator(bucket_it, it);
            }

            return end();
        }

        const_iterator find(const key_type &key) const {
            size_type key_hash = hash_(key) % map_size_;

            const auto &bucket = map_[key_hash];
            auto bucket_it = map_.begin() + key_hash;

            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key)
                    return const_iterator(bucket_it, it);
            }

            return end();
        }

        const_iterator find(key_type &&key) const {
            size_type key_hash = hash_(std::move(key)) % map_size_;

            const auto &bucket = map_[key_hash];
            auto bucket_it = map_.begin() + key_hash;

            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key)
                    return const_iterator(bucket_it, it);
            }

            return end();
        }

        iterator end() noexcept {
            return iterator(map_.end());
        }

        const_iterator end() const noexcept {
            return const_iterator(map_.end());
        }

        iterator begin() noexcept {
            return iterator(map_.begin());
        }

        const_iterator begin() const noexcept {
            return const_iterator(map_.begin());
        }

        void erase(const key_type &key) {
            size_type key_hash = hash_(key) % map_size_;
            auto &bucket = map_[key_hash];

            if (bucket.empty())
                return;

            auto prev_it = bucket.before_begin();
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key) {
                    bucket.erase_after(prev_it);
                    size_--;
                    update_alpha();
                    return;
                }
                ++prev_it;
            }
        }

        void erase(key_type &&key) {
            size_type key_hash = hash_(std::move(key)) % map_size_;
            auto &bucket = map_[key_hash];

            if (bucket.empty())
                return;

            auto prev_it = bucket.before_begin();
            for (auto it = bucket.begin(); it != bucket.end(); ++it) {
                if (it->first == key) {
                    bucket.erase_after(prev_it);
                    size_--;
                    update_alpha();
                    return;
                }
                ++prev_it;
            }
        }

        void erase(iterator it) {
            erase(it->first);
        }

        [[nodiscard]] double alpha() const noexcept { return alpha_; }
        [[nodiscard]] size_type size() const noexcept { return size_; }

    private:
        using map_type = std::vector<std::forward_list<value_type>>;

        size_type map_size_ = 50;
        size_type size_ = 0;

        map_type map_;
        hash_type hash_;

        double alpha_ = size_ / static_cast<double>(map_size_);
        const double resize_alpha_ = 0.75;

        void resize() {
            size_type new_map_size = map_size_ * 2;
            map_type new_map(new_map_size);

            for (auto &bucket : map_) {
                for (const auto &kv : bucket) {
                    size_type key_hash = hash_(kv.first) % new_map_size;
                    new_map[key_hash].emplace_front(std::move(kv));
                }
            }

            map_size_ = new_map_size;
            map_ = std::move(new_map);
            update_alpha();
        }

        void update_alpha() noexcept {
            alpha_ = size_ / static_cast<double>(map_size_);
            if (alpha_ > resize_alpha_)
                resize();
            alpha_ = size_ / static_cast<double>(map_size_);
        }
    };
}

#endif //TRANSACTIONS_LIBRARY_CPP_HASH_TABLE_H
