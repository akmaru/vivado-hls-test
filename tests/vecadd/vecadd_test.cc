#include <stdint.h>

#include <cstdio>

#include <hls_stream.h>

#include <operator.h>
#include <test_util.h>

#include "vecadd_hls.h"
#include "vecadd_ref.h"

using namespace vhlstest;

int main() {
  const int32_t width = 4;
  const int32_t height = 4;

  hls::stream<DType> src0("src0"), src1("src1"), dst("dst");
  DType src0_ref[width * height], src1_ref[width * height],
      dst_ref[width * height];

  const std::vector<int32_t> extents = to_vector(width, height);

  init(src0, extents);
  init(src1, extents);
  init(src0_ref, extents);
  init(src1_ref, extents);

  vecadd_hls(src0, src1, width, height, dst);
  vecadd_ref(src0_ref, src1_ref, width, height, dst_ref);

  if (!verify(dst, dst_ref, extents)) {
    printf("%sFailed%s\n", red, reset);
    return 1;
  }

  printf("%sSucceed!%s\n", green, reset);
  return 0;
}
