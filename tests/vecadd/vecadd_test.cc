#include <stdint.h>

#include <cstdio>

#include <hls_stream.h>

#include <operator.h>
#include <test_util.h>

#include "copy_ref.h"
#include "copy_hls.h"

using namespace HlsTest;

int main()
{
    const int32_t width = 4;
    const int32_t height = 4;

    hls::stream<DType> src("in"), dst("out");
    DType src_ref[width * height], dst_ref[width * height];

    const std::vector<int32_t> extents = to_vector(width, height);

    init(src, extents);
    init(src_ref, extents);

    copy_hls(src, dst, width, height);
    copy_ref(src_ref, dst_ref, width, height);

    if (!verify(dst, dst_ref, extents)) {
        printf("  %sFailed%s\n", red, reset);
        return 1;
    }

    printf("  %sSucceed!%s\n", green, reset);
    return 0;
}
