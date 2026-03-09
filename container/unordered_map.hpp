#pragma once

#include <unordered_map>

namespace atlas {

template <class Key, class T, class Hash = std::hash<Key>, class Equal = std::equal_to<Key>>
using unordered_map = std::unordered_map<Key, T, Hash, Equal>;

}  // namespace atlas

