#include "copy_hls.h"

#include <stdint.h>

#include <hls_stream.h>

namespace vhlstest {
namespace {

template <typename T>
void copy(hls::stream<T>& src, const int32_t width, const int32_t height,
          hls::stream<T>& dst) {
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
#pragma HLS pipeline II = 1
      const T out = src.read();
      dst.write(out);
    }
  }
}

}  // namespace
}  // namespace vhlstest

void copy_hls(hls::stream<DType>& src, int32_t width, int32_t height,
              hls::stream<DType>& dst) {
#pragma HLS dataflow
#pragma HLS interface axis port = src
#pragma HLS interface s_axilite port = width bundle = BUS_AXI4LS
#pragma HLS interface s_axilite port = height bundle = BUS_AXI4LS
#pragma HLS interface axis port = dst

  vhlstest::copy<DType>(src, width, height, dst);
}
