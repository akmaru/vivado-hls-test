#ifndef HLS_TEST_COPY_COPY_REF_H
#define HLS_TEST_COPY_COPY_REF_H

namespace HlsTest {
namespace {

template<typename T>
void copy_ref(const T* src, T* dst, int32_t width, int32_t height)
{
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            const int32_t index = x + y * width;
            dst[index] = src[index];
        }
    }
}

} // namespace anonymous
} // namespace HlsTest

#endif // HLS_TEST_COPY_COPY_REF_H
