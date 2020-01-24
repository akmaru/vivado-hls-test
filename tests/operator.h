#ifndef VHLSTEST_OPERATOR_H
#define VHLSTEST_OPERATOR_H

namespace vhlstest {
namespace {

template <typename T>
T clamp(T v, T lower, T upper) {
#pragma HLS inline
  if (v < lower) {
    return lower;
  } else if (v > upper) {
    return upper;
  }

  return v;
}

template <typename T>
bool overflow(T src, T increase, T extent) {
#pragma HLS inline
  return src + increase >= extent;
}

template <typename T>
T increment_overflow(T src, T increase, T extent) {
#pragma HLS inline
  return src + increase - (overflow(src, increase, extent) ? extent : 0);
}

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_OPERATOR_H
