#include "atlas/container/unordered_set.hpp"
#include <gtest/gtest.h>

TEST(UnorderedSetTest, Basic) {
    atlas::unordered_set<int> set;
    set.insert(1);
    set.insert(2);
    set.insert(3);
    EXPECT_EQ(set.size(), 3);
    EXPECT_EQ(set.find(1), set.end());
    EXPECT_EQ(set.find(2), set.end());
    EXPECT_EQ(set.find(3), set.end());
}