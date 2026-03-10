#pragma once

#include <exception>
#include <iostream>
#include <format>

namespace atlas {

class unimplemented_error : public std::exception {
public:
  explicit unimplemented_error(const char* msg) : msg_("[Atlas][Unimplemented] ") { msg_ += msg; }
  explicit unimplemented_error(const std::string& msg) : msg_("[Atlas][Unimplemented] ") { msg_ += msg; }
  const char* what() const noexcept override { return msg_.c_str(); }
private:
  std::string msg_;
};

namespace detail {
  template <typename... Args>
  void debug_print(std::format_string<Args...> fmt, Args&&... args) {
    std::string message = std::format("[Atlas][Debug]\n\t{}", std::vformat(fmt.get(), std::make_format_args(args...)));
    std::cerr << message << std::endl;
  }
} // namespace atlas::detail
} // namespace atlas

#define ATLAS_DISABLE_COPY(Type)     \
  Type(const Type&) = delete;        \
  Type& operator=(const Type&) = delete

#define ATLAS_DISABLE_MOVE(Type) \
  Type(Type&&) = delete;         \
  Type& operator=(Type&&) = delete

#define ATLAS_DEFAULT_MOVE(Type) \
  Type(Type&&) = default;        \
  Type& operator=(Type&&) = default

#define ATLAS_DEFAULT_COPY(Type) \
  Type(const Type&) = default;   \
  Type& operator=(const Type&) = default;

#define ATLAS_UNIMPLEMENTED(message) \
  throw ::atlas::unimplemented_error(message)

#define ATLAS_UNIMPLEMENTED_FUNCTION(message) \
  ATLAS_UNIMPLEMENTED(                        \
    std::string("[Function] ") + message +    \
    "\n\t" + __PRETTY_FUNCTION__              \
  )

#ifdef NDEBUG
// Do not work in release mode.
#define ATLAS_DEBUG(format, ...) ((void)0)
#else
// Calls std::format and print it to std::cerr.
// Only works in debug mode.
// Example:
//   std::string format_str = "Value: {}";
//   ATLAS_DEBUG(format_str, 42);
#define ATLAS_DEBUG(format, ...) \
  ::atlas::detail::debug_print(format, ##__VA_ARGS__)
#endif