#ifndef VHLSTEST_TESTUTIL_H
#define VHLSTEST_TESTUTIL_H

#include <stdint.h>

#include <cstdio>
#include <functional>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <hls_stream.h>

#include <vector_util.h>

namespace vhlstest {
namespace {

const char* red = "\u001b[31m";
const char* green = "\u001b[32m";
const char* reset = "\u001b[0m";

template <typename T>
void init(hls::stream<T>& port, const std::vector<int32_t>& extents) {
  const int32_t size = std::accumulate(extents.begin(), extents.end(), 1,
                                       std::multiplies<int32_t>());

  for (int i = 0; i < size; i++) {
    port << static_cast<T>(i);
  }
}

template <typename T>
void init(T* buf, const std::vector<int32_t>& extents) {
  const int32_t size = std::accumulate(extents.begin(), extents.end(), 1,
                                       std::multiplies<int32_t>());

  for (int i = 0; i < size; i++) {
    buf[i] = static_cast<T>(i);
  }
}

template <typename T>
bool verify(hls::stream<T>& port, const T* buf,
            const std::vector<int32_t>& extents) {
  bool is_correct = true;
  const int32_t size = std::accumulate(extents.begin(), extents.end(), 1,
                                       std::multiplies<int32_t>());

  for (int i = 0; i < size; i++) {
    const T hls_v = port.read();
    const T ref_v = buf[i];

    if (hls_v != ref_v) {
      const std::vector<int32_t> indices = dimensionalize(i, extents);
      const std::string is = vec2str(indices);
      std::printf("Error%s : HLS = %d, REF = %d\n", is.c_str(),
                  static_cast<int32_t>(hls_v), static_cast<int32_t>(ref_v));
      is_correct = false;
    }
  }

  return is_correct;
}

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_TESTUTIL_H
