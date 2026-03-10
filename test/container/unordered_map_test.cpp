#include "atlas/container/unordered_map.hpp"
#include <gtest/gtest.h>

TEST(UnorderedMapTest, Basic) {
    atlas::unordered_map<int, int> map;
    map[1] = 1;
    map[2] = 2;
    map[3] = 3;
    EXPECT_EQ(map.size(), 3);
    EXPECT_EQ(map[1], 1);
    EXPECT_EQ(map[2], 2);
    EXPECT_EQ(map[3], 3);
}