#pragma once

#include <shared_mutex>

#include "atlas/common/config.hpp"

namespace atlas {

using shared_lock = std::shared_mutex;

class shared_mutex {
public:
  void lock() { shared_mutex_.lock(); }
  void unlock() { shared_mutex_.unlock(); }
  void lock_shared() { shared_mutex_.lock_shared(); }
  void unlock_shared() { shared_mutex_.unlock_shared(); }
  bool try_lock() { return shared_mutex_.try_lock(); }
  bool try_lock_shared() { return shared_mutex_.try_lock_shared(); }
private:
  struct EmptySharedMutex {
    void lock() noexcept {}
    void unlock() noexcept {}
    void lock_shared() noexcept {}
    void unlock_shared() noexcept {}
    bool try_lock() noexcept { return true; }
    bool try_lock_shared() noexcept { return true; }
  };

  [[no_unique_address]]
  std::conditional_t<kConcurrencyEnabled, std::shared_mutex, EmptySharedMutex> shared_mutex_;
};

} // namespace atlas