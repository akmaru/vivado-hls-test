#include "copy_hls.h"

#include <stdint.h>

#include <hls_stream.h>

namespace HlsTest {
namespace {

template<typename T>
void copy(hls::stream<T>& src, hls::stream<T>& dst, const int32_t width, const int32_t height)
{
    for (int c = 0; c < height-width; c++) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
#pragma HLS pipeline II=1
            const T out = src.read() + x + y + c;
            dst.write(out);
        }
    }
    }
}

} // namespace anonymous
} // namespace HlsTest


void copy_hls(hls::stream<DType>& src, hls::stream<DType>& dst, int32_t width, int32_t height)
{
#pragma HLS dataflow
#pragma HLS interface axis port=src
#pragma HLS interface axis port=dst
#pragma HLS interface s_axilite port=width bundle=BUS_AXI4LS
#pragma HLS interface s_axilite port=height bundle=BUS_AXI4LS

    HlsTest::copy<DType>(src, dst, width, height);
}
