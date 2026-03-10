#pragma once

#include <cstdlib>
#include <format>
#include <iostream>
#include <string_view>

namespace atlas {
inline void AssertFail(std::string_view expr,
                       std::string_view file,
                       int line,
                       std::string_view message) {
  std::cerr << "[Atlas][Assertion failure]\n\t" << expr << " at " << file << ":" << line;
  if(!message.empty()) {
    std::cerr << " - " << message;
  }
  std::cerr << std::endl;
  std::abort();
}

template <class... Args>
inline void AssertFormatted(bool expr,
                            std::string_view expr_str,
                            std::string_view file,
                            int line,
                            std::string_view fmt,
                            Args &&... args) {
  if(!expr) {
    auto msg = std::vformat(fmt, std::make_format_args(args...));
    AssertFail(expr_str, file, line, msg);
  }
}

template <class... Args>
inline void Debugf(std::string_view fmt, Args &&... args) {
  auto msg = std::vformat(fmt, std::make_format_args(args...));
  std::clog << msg << std::endl;
}
} // namespace atlas

#define ATLAS_ASSERT(expr, fmt, ...)                              \
    do {                                                          \
        ::atlas::AssertFormatted((expr), #expr, __FILE__, __LINE__, (fmt), ##__VA_ARGS__); \
    } while (false)

#define ATLAS_DEBUG(fmt, ...)                                     \
    do {                                                          \
        ::atlas::Debugf((fmt), ##__VA_ARGS__);                    \
    } while (false)
