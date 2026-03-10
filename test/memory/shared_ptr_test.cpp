#include "atlas/memory/memory.hpp"
#include <gtest/gtest.h>

TEST(SharedPtrTest, Basic) {
    atlas::shared_ptr<int> ptr1(new int(1));
    atlas::shared_ptr<int> ptr2(ptr1);
    EXPECT_EQ(*ptr1, 1);
    EXPECT_EQ(*ptr2, 1);
    EXPECT_EQ(ptr1.use_count(), 2);
    EXPECT_EQ(ptr2.use_count(), 2);
}