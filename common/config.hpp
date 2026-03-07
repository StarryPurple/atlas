#pragma once

namespace atlas {

// Compile-time switches and global configuration.
//
// Concurrency:
// - By default, Altar assumes concurrency is enabled (thread-safe).
// - If CMake option ALTAR_ENABLE_CONCURRENCY is set to OFF (or ONLINE_JUDGE is defined),
//   ALTAR_DISABLE_CONCURRENCY will be defined, optimizing for single-threaded use.
#if defined(ALTAR_DISABLE_CONCURRENCY) || defined(ONLINE_JUDGE)
inline constexpr bool kConcurrencyEnabled = false;
#else
inline constexpr bool kConcurrencyEnabled = true;
#endif

}  // namespace atlas
