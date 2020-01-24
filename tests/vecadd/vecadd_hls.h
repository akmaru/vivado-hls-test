#ifndef HLS_TEST_COPY_COPY_HLS_H
#define HLS_TEST_COPY_COPY_HLS_H

#include "copy_defs.h"

#include <stdint.h>

#include <hls_stream.h>

void copy_hls(hls::stream<DType>& src, hls::stream<DType>& dst, int32_t width, int32_t height);

#endif // HLS_TEST_COPY_COPY_HLS_H
