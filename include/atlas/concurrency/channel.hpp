#pragma once

#include "atlas/common/util.hpp"
#include "atlas/concurrency/mutex.hpp"
#include "atlas/concurrency/condition_variable.hpp"
#include "atlas/container/queue.hpp"

namespace atlas {

// A message / task channel with theoretically infinite buffer.
template <typename T>
class channel {
public:
  channel() = default;
  ~channel() = default;

  // Get (move out) the oldest element in the channel.
  // It shall block the thread until at least one element is available.
  T get() {
    ATLAS_UNIMPLEMENTED_FUNCTION();
  }

  // Pass one element into the channel.
  // With infinite buffer, this operation shall always succeed.
  
  void put(const T& t) {
    ATLAS_UNIMPLEMENTED_FUNCTION();
  }
  void put(T&& t) {
    ATLAS_UNIMPLEMENTED_FUNCTION();
  }

private:
  [[no_unique_address]] mutex mtx_;
  [[no_unique_address]] condition_variable cv_;
  queue<T> buf_;
};

}
