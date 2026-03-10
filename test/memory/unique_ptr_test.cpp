#include "atlas/memory/memory.hpp"
#include <gtest/gtest.h>

TEST(UniquePtrTest, Basic) {
    atlas::unique_ptr<int> ptr(new int(1));
    EXPECT_EQ(*ptr, 1);
}