#pragma once

#include <stdexcept>
#include <string>

namespace atlas {
// used when using an index out of capacity.
// Examples: vector.at(size()), map::at(invalid_key), *map.end(), *map::iterator(), queue{}.front()
// Yet sometimes you may want these to be undefined behaviours rather than exceptions.
class out_of_range: public std::out_of_range {
public:
  out_of_range()
    : std::out_of_range("[Atlas][Exception][Out of range]") {}

  explicit out_of_range(const std::string& what_arg)
    : std::out_of_range("[Atlas][Exception][Out of range]\n\t" + what_arg) {}
};

// Used when arguments logically unfit the current circumstance are passed in.
// It implies that some pre-defined attributes do not match between the arguments and the function / object.
// Examples: map1.erase(map2.begin()), Student::set_age(150)
class invalid_argument: public std::invalid_argument {
public:
  invalid_argument()
    : std::invalid_argument("[Atlas][Exception][Invalid argument]") {}

  explicit invalid_argument(const std::string& what_arg)
    : std::invalid_argument("[Atlas][Exception][Invalid argument]\n\t" + what_arg) {}
};

class runtime_error: public std::runtime_error {
public:
  runtime_error()
    : std::runtime_error("[Atlas][Exception][Runtime error]") {}

  explicit runtime_error(const std::string& what_arg)
    : std::runtime_error("[Atlas][Exception][Runtime error]\n\t" + what_arg) {}
};
} // namespace atlas
