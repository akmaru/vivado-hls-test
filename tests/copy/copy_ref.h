#ifndef VHLS_TEST_COPY_COPY_REF_H
#define VHLS_TEST_COPY_COPY_REF_H

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

#endif  // VHLS_TEST_COPY_COPY_REF_H
