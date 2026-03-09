#pragma once

#include <unordered_set>

namespace atlas {

// Placeholder for a custom hash set implementation.
// Students are expected to design and implement this container.
template <class Value, class Hash = std::hash<Value>, class Equal = std::equal_to<Value>>
using unordered_set = std::unordered_set<Value, Hash, Equal>;

}  // namespace atlas

