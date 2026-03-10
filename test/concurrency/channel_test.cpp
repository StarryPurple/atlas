#include "atlas/concurrency/channel.hpp"
#include "gtest/gtest.h"

TEST(ChannelTests, ConstructorTest) {
  atlas::channel<int> channel;
  try {
    channel.put(1);
  } catch(atlas::unimplemented_error& e) {
    std::cout << e.what();
  }
}