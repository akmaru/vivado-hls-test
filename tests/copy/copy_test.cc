#include <stdint.h>

#include <cstdio>

#include <hls_stream.h>

#include <operator.h>
#include <test_util.h>

#include "copy_hls.h"
#include "copy_ref.h"

using namespace vhlstest;

int main() {
  const int32_t width = 4;
  const int32_t height = 4;

  hls::stream<DType> src("src"), dst("dst");
  DType src_ref[width * height], dst_ref[width * height];

  const std::vector<int32_t> extents = to_vector(width, height);

  init(src, extents);
  init(src_ref, extents);

  copy_hls(src, width, height, dst);
  copy_ref(src_ref, width, height, dst_ref);

  if (!verify(dst, dst_ref, extents)) {
    printf("%sFailed%s\n", red, reset);
    return 1;
  }

  printf("%sSucceed!%s\n", green, reset);
  return 0;
}
