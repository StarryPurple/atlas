#pragma once

#include <type_traits>
#include <memory>

#include "atlas/common/config.hpp"
#include "atlas/common/util.hpp"

namespace atlas {

// no fetch_add and operator++ and other things...
// Maybe you can implement it? Leave it as a bonus.
template <typename T> requires std::is_trivially_copyable_v<T>
class atomic {
public:
  atomic() noexcept = default;
  constexpr explicit atomic(T desired) noexcept: value_(desired) {}

  ATLAS_DISABLE_COPY(atomic);
  // didn't see move ctor explicitly disabled in std::atomic... Whatever.
  ATLAS_DISABLE_MOVE(atomic);

  T load(std::memory_order m = std::memory_order_seq_cst) const noexcept {
    if constexpr (kConcurrencyEnabled) {
      return value_.load(m);
    } else {
      return value_;
    }
  }

  void store(T desired, std::memory_order m = std::memory_order_seq_cst) noexcept {
    if constexpr (kConcurrencyEnabled) {
      value_.store(desired, m);
    } else {
      value_ = desired;
    }
  }

  T exchange(T desired, std::memory_order m = std::memory_order_seq_cst) noexcept {
    if constexpr (kConcurrencyEnabled) {
      return value_.exchange(desired, m);
    } else {
      T old = value_;
      value_ = desired;
      return old;
    }
  }

  bool compare_exchange_weak(T& expected, T desired, std::memory_order m = std::memory_order_seq_cst) noexcept {
    if constexpr (kConcurrencyEnabled) {
      return value_.compare_exchange_weak(expected, desired, m);
    } else {
      if (value_ == expected) {
        value_ = desired;
        return true;
      }
      expected = value_;
      return false;
    }
  }

  bool compare_exchange_strong(T& expected, T desired, std::memory_order m = std::memory_order_seq_cst) noexcept {
    if constexpr (kConcurrencyEnabled) {
      return value_.compare_exchange_strong(expected, desired, m);
    } else {
      if (value_ == expected) {
        value_ = desired;
        return true;
      }
      expected = value_;
      return false;
    }
  }

  // tired of implementing the versions of CAS with success/failure memory order parameters.

  T operator=(T desired) noexcept { store(desired); return desired; } // NOLINT: value rather than reference shall be returned
  explicit operator T() const noexcept { return load(); }

private:
  std::conditional_t<kConcurrencyEnabled, std::atomic<T>, T> value_;
};
} // namespace atlas