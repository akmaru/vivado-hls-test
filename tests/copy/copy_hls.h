#ifndef VHLSTEST_COPY_HLS_H
#define VHLSTEST_COPY_HLS_H

#include <stdint.h>

#include <hls_stream.h>

#include "copy_defs.h"

void copy_hls(hls::stream<DType>& src, int32_t width, int32_t height,
              hls::stream<DType>& dst);

#endif  // VHLSTEST_COPY_HLS_H
