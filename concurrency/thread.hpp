#pragma once

#include <thread>
#include <functional>

#include "common/config.hpp"
#include "common/util.hpp"

namespace atlas {
class thread {
public:
  using id = std::thread::id;

  thread() noexcept = default;
  ~thread() = default; // if joinable: terminate.

  template <typename Func, typename... Args>
    requires std::is_invocable_v<Func, Args...>
  explicit thread(Func &&func, Args &&... args) {
    if constexpr(kConcurrencyEnabled) {
      thread_ = std::thread(std::forward<Func>(func), std::forward<Args>(args)...);
    } else {
      // invoke the task instantly
      std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
    }
  }

  SWIRL_DISABLE_COPY(thread);
  SWIRL_DEFAULT_MOVE(thread);

  bool joinable() const noexcept { return thread_.joinable(); }
  void join() { thread_.join(); }
  void detach() { thread_.detach(); }
  id get_id() const noexcept { return thread_.get_id(); }

  // no need to cover anything.
  static unsigned int hardware_concurrency() noexcept {
    return std::thread::hardware_concurrency();
  }

private:
  struct EmptyThread {
    bool joinable() const noexcept { return false; }
    void join() {}
    void detach() {}
    id get_id() const noexcept { return id{}; }
  };

  [[no_unique_address]]
  std::conditional_t<kConcurrencyEnabled, std::thread, EmptyThread> thread_;
};
} // namespace atlas