#include "atlas/container/array.hpp"
#include <gtest/gtest.h>

TEST(ArrayTest, Basic) {
    atlas::array<int, 3> arr = {1, 2, 3};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[1], 2);
    EXPECT_EQ(arr[2], 3);
}