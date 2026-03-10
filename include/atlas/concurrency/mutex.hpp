#pragma once

#include <mutex>

#include "atlas/common/config.hpp"

namespace atlas {

template <typename Mutex>
using unique_lock = std::unique_lock<Mutex>;

template <typename Mutex>
using lock_guard = std::lock_guard<Mutex>;

template <typename... MutexTypes>
using scoped_lock = std::scoped_lock<MutexTypes...>;

class mutex {
public:
  void lock() { mutex_.lock(); }
  void unlock() { mutex_.unlock(); }
  bool try_lock() { return mutex_.try_lock(); }

private:
  struct EmptyMutex {
    void lock() noexcept {}
    void unlock() noexcept {}
    bool try_lock() noexcept { return true; }
  };

  [[no_unique_address]]
  std::conditional_t<kConcurrencyEnabled, std::mutex, EmptyMutex> mutex_;
};
} // namespace atlas