#ifndef VHLSTEST_VECADD_HLS_H
#define VHLSTEST_VECADD_HLS_H

#include <stdint.h>

#include <hls_stream.h>

#include "vecadd_defs.h"

void vecadd_hls(hls::stream<DType>& src0, hls::stream<DType>& src1,
                int32_t width, int32_t height, hls::stream<DType>& dst);

#endif  // VHLSTEST_VECADD_HLS_H
