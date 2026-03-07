#pragma once

#include <condition_variable>

#include "common/config.hpp"
#include "concurrency/mutex.hpp"

namespace atlas {


class condition_variable {
public:
  void notify_one() noexcept { cv_any_.notify_one(); }
  void notify_all() noexcept { cv_any_.notify_all(); }
  void wait(unique_lock<mutex>& lock) noexcept {
    cv_any_.wait(lock);
  }
  template <typename Predicate>
  void wait(unique_lock<mutex>& lock, Predicate pred) {
    cv_any_.wait(lock, pred);
  }

private:
  struct EmptyCV {
    void notify_one() {}
    void notify_all() {}
    void wait(unique_lock<mutex>) noexcept {}
    template <typename Predicate>
    void wait(unique_lock<mutex>, Predicate) {} // pred shall be true... Ahh, no check here.
  };

  [[no_unique_address]]
  std::conditional_t<kConcurrencyEnabled, std::condition_variable_any, EmptyCV> cv_any_;
};

// implement condition_variable_any?

} // namespace atlas