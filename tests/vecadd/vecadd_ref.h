#ifndef VHLSTEST_VECADD_REF_H
#define VHLSTEST_VECADD_REF_H

#include <stdint.h>

namespace vhlstest {
namespace {

template <typename DType>
void vecadd_ref(const DType* src0, const DType* src1, int32_t width,
                int32_t height, DType* dst) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      const int32_t index = x + y * width;
      dst[index] = src0[index] + src1[index];
    }
  }
}

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_VECADD_REF_H
