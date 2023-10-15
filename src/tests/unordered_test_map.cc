#include "unordered_map.h"

#include <gtest/gtest.h>


TEST(unordered_map, default_constructor) {
    ttl::unordered_map<int, int> map;

    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty() == true);
}

TEST(unordered_map, insert_copy) {
    ttl::unordered_map<int, int> map;
    std::pair<int, int> kv {1, 1};
    map.insert(kv);

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(unordered_map, insert_move) {
    ttl::unordered_map<int, int> map;
    map.insert({1, 1});

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(unordered_map, operator_copy) {
    ttl::unordered_map<int, int> map;
    std::pair<int, int> kv {1, 1};
    map[kv.first] = kv.second;

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(unordered_map, operator_move) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(unordered_map, begin) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;

    auto begin = map.begin();
    ASSERT_TRUE(begin->first == 1);
    ASSERT_TRUE(begin->second == 1);
}

TEST(unordered_map, const_begin) {
    const ttl::unordered_map<int, int> map;

    auto begin = map.begin();
    ASSERT_TRUE(begin == map.end());
}


TEST(unordered_map, capacity) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;

    ASSERT_TRUE(map.capacity() == ttl::detail::unordered_map_size::size(1));
}

TEST(unordered_map, find_copy) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;

    int key = 1;
    auto it = map.find(key);
    ASSERT_TRUE(it->first == 1);
    ASSERT_TRUE(it->second == 1);
}

TEST(unordered_map, find_move) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;

    auto it = map.find(1);
    ASSERT_TRUE(it->first == 1);
    ASSERT_TRUE(it->second == 1);
}

TEST(unordered_map, erase_copy) {
    ttl::unordered_map<int, int> map;
    int key = 1;

    map[key] = 1;


    ASSERT_TRUE(map.erase(key) == true);
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(unordered_map, erase_move) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;


    ASSERT_TRUE(map.erase(1) == true);
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(unordered_map, erase_it) {
    ttl::unordered_map<int, int> map;
    map[1] = 1;

    ASSERT_TRUE(map.erase(map.find(1)) == true);
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(unordered_map, iter_over) {
    ttl::unordered_map<int, int> map;
    for (int i = 0; i != 100; ++i)
        map.insert({i, i + 1});

    for (const auto &[key, value] : map)
        ASSERT_EQ(key + 1, value);

    ASSERT_TRUE(map.size() == 100);
}
