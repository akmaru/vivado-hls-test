#ifndef VHLSTEST_BUNDLE_H
#define VHLSTEST_BUNDLE_H

#include <stdint.h>
#include <cassert>
#include <cstdio>

#include <ap_int.h>

namespace vhlstest {
namespace {

template <typename T, int32_t N>
struct Bundle {
  typedef T native_t;
  typedef ap_uint<8 * sizeof(T)> scalar_t;
  typedef ap_uint<8 * sizeof(T) * N> vector_t;

  vector_t bv;

  Bundle(void)
#if !defined(__SYNTHESIS__)
        : bv(0)
#endif
    {
#pragma HLS inline
    }
    Bundle(vector_t bv_)
        : bv(bv_)
    {
#pragma HLS inline
    }

    Bundle(const T v_[])
    {
#pragma HLS inline
    for (int i = 0; i < N; ++i) {
#pragma HLS unroll
      put(v_[i], i);
    }
  }

  T get(int32_t i) const {
#pragma HLS inline
    assert(0 <= i && i < N);
    scalar_t sv =
        bv.range(8 * sizeof(T) * i + 8 * sizeof(T) - 1, 8 * sizeof(T) * i);
    return *reinterpret_cast<T*>(&sv);
  }

  void put(const T& v, int32_t i) {
#pragma HLS inline
    assert(0 <= i && i < N);
    bv.range(8 * sizeof(T) * i + 8 * sizeof(T) - 1, 8 * sizeof(T) * i) =
        *reinterpret_cast<const scalar_t*>(&v);
  }

  void print(void) {
    printf("[");
    for (int32_t i = 0; i < N; ++i) {
      ::print(get(i));
      printf(", ");
    }
    printf("]");
  }
};

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_BUNDLE_H
