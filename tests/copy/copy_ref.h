#ifndef VHLSTEST_COPY_REF_H
#define VHLSTEST_COPY_REF_H

#include <stdint.h>

namespace vhlstest {
namespace {

template <typename DType>
void copy_ref(const DType* src, int32_t width, int32_t height, DType* dst) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      const int32_t index = x + y * width;
      dst[index] = src[index];
    }
  }
}

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_COPY_REF_H
