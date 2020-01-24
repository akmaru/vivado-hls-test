#ifndef VHLSTEST_VECTOR_UTIL_H
#define VHLSTEST_VECTOR_UTIL_H

#include <stdint.h>

#include <functional>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace vhlstest {
namespace {

template <typename T>
std::vector<T> to_vector(T v) {
  std::vector<T> vec;
  vec.push_back(v);
  return vec;
}

template <typename T>
std::vector<T> to_vector(T v0, T v1) {
  std::vector<T> vec;
  vec.push_back(v0);
  vec.push_back(v1);
  return vec;
}

template <typename T>
std::vector<T> to_vector(T v0, T v1, T v2) {
  std::vector<T> vec;
  vec.push_back(v0);
  vec.push_back(v1);
  vec.push_back(v2);
  return vec;
}

template <typename T>
std::vector<T> to_vector(T v0, T v1, T v2, T v3) {
  std::vector<T> vec;
  vec.push_back(v0);
  vec.push_back(v1);
  vec.push_back(v2);
  vec.push_back(v3);
  return vec;
}

template <typename T>
std::string vec2str(const std::vector<T> vec) {
  std::stringstream ss;

  ss << "(";
  for (std::size_t i = 0; i < vec.size(); i++) {
    ss << vec[i];
    if (i < vec.size() - 1) {
      ss << ", ";
    }
  }
  ss << ")";

  return ss.str();
}

template <typename T>
std::vector<T> dimensionalize(T index, const std::vector<T> extents) {
  std::vector<T> indices(extents);

  for (std::size_t i = 0; i < extents.size(); i++) {
    const T denom = std::accumulate(extents.begin(), extents.begin() + i, 1,
                                    std::multiplies<T>());
    const T dimensionalized = (index / denom) % extents[i];
    indices[i] = dimensionalized;
  }

  return indices;
}

template <typename T>
T flatten(const std::vector<T>& indices, const std::vector<T>& extents) {
  T index = 0;
  for (size_t i = 0; i < indices.size(); i++) {
    const T stride = std::accumulate(extents.begin(), extents.begin() + i, 1,
                                     std::multiplies<T>());
    index += indices[i] * stride;
  }

  return index;
}

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_VECTOR_UTIL_H
