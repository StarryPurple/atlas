#pragma once

#include <memory>
#include "concurrency/atomic.hpp" // need it?

namespace atlas {
// Memory utilities and adapters for smart pointers.
//
// The intent is to provide a centralized place where you can
// redefine or wrap standard smart pointers (unique_ptr, shared_ptr,
// weak_ptr, etc.), and potentially lighter-weight or non-concurrent
// alternatives when atlas::kConcurrencyEnabled is false.
//
// Future extensions may also include more advanced memory management
// primitives (e.g., hazard pointers).
//
// For now, we just simply use typedef.

template <typename T>
using unique_ptr = std::unique_ptr<T>;

template <typename T>
using shared_ptr = std::shared_ptr<T>;

template <typename T>
using weak_ptr = std::weak_ptr<T>;

template <typename T, typename... Args>
  requires (!std::is_array_v<T>) && std::is_constructible_v<T, Args...>
constexpr unique_ptr<T> make_unique(Args &&... args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
  requires std::is_unbounded_array_v<T> && std::is_default_constructible_v<std::remove_extent_t<T>>
constexpr unique_ptr<T> make_unique(std::size_t size) {
  return std::make_unique<T>(size);
}

template <typename T, typename... Args>
  requires (!std::is_array_v<T>) && std::is_constructible_v<T, Args...>
shared_ptr<T> make_shared(Args &&... args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
} // namespace atlas
