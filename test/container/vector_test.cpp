#include "atlas/container/vector.hpp"
#include <gtest/gtest.h>

TEST(VectorTest, Basic) {
    atlas::vector<int> vec;
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    EXPECT_EQ(vec.size(), 3);
    EXPECT_EQ(vec[0], 1);
    EXPECT_EQ(vec[1], 2);
    EXPECT_EQ(vec[2], 3);
}
#include "gtest/gtest.h"  