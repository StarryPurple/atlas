#include "atlas/container/queue.hpp"
#include <gtest/gtest.h>

TEST(QueueTest, Basic) {
    atlas::queue<int> queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    EXPECT_EQ(queue.size(), 3);
    EXPECT_EQ(queue.front(), 1);
    EXPECT_EQ(queue.back(), 3);
}