#include "atlas/container/list.hpp"
#include <gtest/gtest.h>

TEST(ListTest, Basic) {
    atlas::list<int> list;
    list.push_back(1);
    list.push_back(2);
    list.push_back(3);
    EXPECT_EQ(list.size(), 3);
}