#include "map.h"

#include <gtest/gtest.h>


TEST(map, default_constructor) {
    ttl::map<int, int> map;

    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty() == true);
}

TEST(map, copy_constructor) {
    ttl::map<int, int> map;
    map.insert({1, 1});

    ttl::map<int, int> map2 = map;
    ASSERT_EQ(map.size(), map2.size());
    ASSERT_EQ(map.begin()->first, map2.begin()->first);
}

TEST(map, copy_operator) {
    ttl::map<int, int> map;
    map.insert({1, 1});

    ttl::map<int, int> map2;
    map2 = map;
    ASSERT_EQ(map.size(), map2.size());
    ASSERT_EQ(map.begin()->first, map2.begin()->first);
}

TEST(map, move_constructor) {
    ttl::map<int, int> map;
    map.insert({1, 1});

    ttl::map<int, int> map2 = std::move(map);
    ASSERT_EQ(map.size(), 0);
    ASSERT_EQ(map2.size(), 1);
}

TEST(map, move_operator) {
    ttl::map<int, int> map;
    map.insert({1, 1});

    ttl::map<int, int> map2;
    map2 = std::move(map);
    ASSERT_EQ(map.size(), 0);
    ASSERT_EQ(map2.size(), 1);
}

TEST(map, insert_copy) {
    ttl::map<int, int> map;
    std::pair<int, int> kv {1, 1};
    map.insert(kv);

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(map, insert_move) {
    ttl::map<int, int> map;
    map.insert({1, 1});

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(map, operator_copy) {
    ttl::map<int, int> map;
    std::pair<int, int> kv {1, 1};
    map[kv.first] = kv.second;

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(map, operator_move) {
    ttl::map<int, int> map;
    map[1] = 1;

    ASSERT_TRUE(map.size() == 1);
    ASSERT_TRUE(map.empty() == false);
}

TEST(map, begin) {
    ttl::map<int, int> map;
    map[1] = 1;

    auto begin = map.begin();
    ASSERT_TRUE(begin->first == 1);
    ASSERT_TRUE(begin->second == 1);
}

TEST(map, find_copy) {
    ttl::map<int, int> map;
    map[1] = 1;

    int key = 1;
    auto it = map.find(key);
    ASSERT_TRUE(it->first == 1);
    ASSERT_TRUE(it->second == 1);
}

TEST(map, find_move) {
    ttl::map<int, int> map;
    map[1] = 1;

    auto it = map.find(1);
    ASSERT_TRUE(it->first == 1);
    ASSERT_TRUE(it->second == 1);
}

TEST(map, erase_copy) {
    ttl::map<int, int> map;
    int key = 1;

    map[key] = 1;

    map.erase(key);
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(map, erase_move) {
    ttl::map<int, int> map;
    map[1] = 1;

    map.erase(1);
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(map, erase_it) {
    ttl::map<int, int> map;
    map[1] = 1;

    map.erase(map.find(1));
    ASSERT_TRUE(map.size() == 0);
    ASSERT_TRUE(map.empty());
}

TEST(map, iter_over) {
    ttl::map<int, int> map;
    for (int i = 0; i != 100; ++i)
        map.insert({i, i + 1});

    for (const auto &[key, value] : map)
        ASSERT_EQ(key + 1, value);

    ASSERT_TRUE(map.size() == 100);
}

TEST(map, erase_over) {
    ttl::map<int, int> map;
    for (int i = 0; i != 100; ++i)
        map.insert({i, i + 1});

    for (int i = 0; i != 100; ++i)
        map.erase(i);

    ASSERT_TRUE(map.size() == 0);
}
