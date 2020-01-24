#ifndef VHLSTEST_LINEBUFFER_H
#define VHLSTEST_LINEBUFFER_H

#include <stdint.h>
#include <iomanip>
#include <iostream>

#include <operator.h>

namespace vhlstest {
namespace {

template <typename T, int32_t EXTENT0_MAX, int32_t EXTENT1_MAX,
          int32_t W_EXTENT0, int32_t W_EXTENT1>
struct LineBuffer2d {
#define BRAM_EXTENT0_MAX (EXTENT0_MAX - W_EXTENT0 + 1)
#define BRAM_EXTENT1_MAX (W_EXTENT1)

  T regs[W_EXTENT1][W_EXTENT0];
  T brams[BRAM_EXTENT1_MAX][BRAM_EXTENT0_MAX];

  int32_t head;
  int32_t tail;

  LineBuffer2d(int32_t extent0, int32_t extent1) {
#pragma HLS inline
#pragma HLS array_partition variable = regs complete dim = 0
#pragma HLS array_partition variable = brams complete dim = 1
#pragma HLS resource variable = brams core = RAM_2P_BRAM
    head = 0;
    tail = extent0 - W_EXTENT0;
  }

  void shift() {
#pragma HLS inline
#pragma HLS dependence variable = brams intra false
    for (int y = 0; y < W_EXTENT1 - 1; y++) {
#pragma HLS unroll
      brams[y][tail] = regs[y][0];
      for (int x = 0; x < W_EXTENT0 - 1; x++) {
#pragma HLS unroll
        regs[y][x] = regs[y][x + 1];
      }
      regs[y][W_EXTENT0 - 1] = brams[y + 1][head];
    }

    {
      const int y = W_EXTENT0 - 1;
      brams[y][tail] = regs[y][0];
      for (int x = 0; x < W_EXTENT0 - 1; x++) {
#pragma HLS unroll
        regs[y][x] = regs[y][x + 1];
      }
    }

    head = head < BRAM_EXTENT0_MAX - 1 ? head + 1 : 0;
    tail = tail < BRAM_EXTENT0_MAX - 1 ? tail + 1 : 0;
  }

  void rotate1() {
#pragma HLS inline
    for (int y = 0; y < W_EXTENT1; y++) {
#pragma HLS unroll
#pragma HLS dependence variable = brams intra false
#pragma HLS dependence variable = brams inter RAW distance = 2 true
      brams[y][tail] = regs[y][0];
      for (int x = 0; x < W_EXTENT0 - 1; x++) {
#pragma HLS unroll
        regs[y][x] = regs[y][x + 1];
      }
      regs[y][W_EXTENT0 - 1] = brams[y][head];
    }

    head = head < BRAM_EXTENT0_MAX - 1 ? head + 1 : 0;
    tail = tail < BRAM_EXTENT0_MAX - 1 ? tail + 1 : 0;
  }

  void shift_or_rotate1(T v, bool is_shift) {
#pragma HLS inline
#pragma HLS dependence variable = brams intra false
#pragma HLS dependence variable = brams inter RAW distance = 2 true
    for (int y = 0; y < W_EXTENT1 - 1; y++) {
#pragma HLS unroll
      brams[y][tail] = regs[y][0];
      for (int x = 0; x < W_EXTENT0 - 1; x++) {
#pragma HLS unroll
        regs[y][x] = regs[y][x + 1];
      }
      regs[y][W_EXTENT0 - 1] = is_shift ? brams[y + 1][head] : brams[y][head];
    }

    {
      const int y = W_EXTENT0 - 1;
      brams[y][tail] = regs[y][0];
      for (int x = 0; x < W_EXTENT0 - 1; x++) {
#pragma HLS unroll
        regs[y][x] = regs[y][x + 1];
      }
      regs[y][W_EXTENT0 - 1] = is_shift ? v : brams[y][head];
    }

    head = head < BRAM_EXTENT0_MAX - 1 ? head + 1 : 0;
    tail = tail < BRAM_EXTENT0_MAX - 1 ? tail + 1 : 0;
  }

  void store(T v, int32_t x, int32_t y) {
#pragma HLS inline
    regs[y][x] = v;
  }

  void store(T v) {
#pragma HLS inline
    store(v, W_EXTENT0 - 1, W_EXTENT1 - 1);
  }

  T load(int32_t x, int32_t y) const {
#pragma HLS inline
    return regs[y][x];
  }

  T load() const {
#pragma HLS inline
    return load(W_EXTENT0 - 1, W_EXTENT1 - 1);
  }

  void print() const {
    const int32_t bram_extent0 = abs(tail - head);

    for (int y = 0; y < W_EXTENT1; y++) {
      std::cout << "{";

      for (int x = 0; x < bram_extent0; x++) {
        const int bram_x = (x + head) % BRAM_EXTENT0_MAX;
        std::cout << std::setw(3) << static_cast<int64_t>(brams[y][bram_x]);
        if (x < bram_extent0 - 1) {
          std::cout << ", ";
        }
      }

      std::cout << "}, [";

      for (int x = 0; x < W_EXTENT0; x++) {
        std::cout << std::setw(3) << static_cast<int64_t>(regs[y][x]);
        if (x < W_EXTENT0 - 1) {
          std::cout << ", ";
        }
      }

      if (y < W_EXTENT1 - 1) {
        std::cout << "], " << std::endl;
      } else {
        std::cout << "]" << std::endl;
      }
    }
  }
};

template <typename T, int32_t EXTENT0_MAX, int32_t EXTENT1_MAX,
          int32_t EXTENT2_MAX, int32_t W_EXTENT0, int32_t W_EXTENT1,
          int32_t W_EXTENT2>
struct LineBuffer3d {
#define BRAM0_EXTENT ((EXTENT0_MAX - W_EXTENT0) * 1 + 1)
#define BRAM1_EXTENT ((EXTENT1_MAX - W_EXTENT1) * EXTENT0_MAX + 1)

  T regs[W_EXTENT2][W_EXTENT1][W_EXTENT0];
  T brams0[W_EXTENT2][W_EXTENT1][BRAM0_EXTENT];
  T brams1[W_EXTENT2][BRAM1_EXTENT];

  int32_t extent0, extent1, extent2;
  int32_t head0, head1;
  int32_t tail0, tail1;

  LineBuffer3d(int32_t e0, int32_t e1, int32_t e2) {
#pragma HLS inline
#pragma HLS array_partition variable = regs complete dim = 0
#pragma HLS array_partition variable = brams0 complete dim = 1
#pragma HLS array_partition variable = brams0 complete dim = 2
#pragma HLS array_partition variable = brams1 complete dim = 1
#pragma HLS resource variable = brams0 core = RAM_2P_BRAM
#pragma HLS resource variable = brams1 core = RAM_2P_BRAM
    extent0 = e0;
    extent1 = e1;
    extent2 = e2;
    head0 = 0;
    head1 = 0;
    tail0 = (extent0 - W_EXTENT0) * 1;
    tail1 = (extent1 - W_EXTENT1) * extent0;
  }

  void increment_ptrs_d0() {
#pragma HLS inline
    head0 = increment_overflow(head0, 1, BRAM0_EXTENT);
    tail0 = increment_overflow(tail0, 1, BRAM0_EXTENT);
  }

  void increment_ptrs_d1() {
#pragma HLS inline
    head1 = increment_overflow(head1, 1, BRAM1_EXTENT);
    tail1 = increment_overflow(tail1, 1, BRAM1_EXTENT);
  }

  void increment_ptrs() {
#pragma HLS inline
    increment_ptrs_d0();
    increment_ptrs_d1();
  }

  void shift() {
#pragma HLS inline
#pragma HLS dependence variable = brams0 intra false
#pragma HLS dependence variable = brams1 intra false

    // original implementation
    for (int i2 = 0; i2 < W_EXTENT2; i2++) {
#pragma HLS unroll
      brams1[i2][tail1] = brams0[i2][0][head0];
      for (int i1 = 0; i1 < W_EXTENT1; i1++) {
#pragma HLS unroll
        brams0[i2][i1][tail0] = regs[i2][i1][0];
        for (int i0 = 0; i0 < W_EXTENT0; i0++) {
#pragma HLS unroll
          regs[i2][i1][i0] =
              (i0 < W_EXTENT0 - 1
                   ? regs[i2][i1][i0 + 1]
                   : (i1 < W_EXTENT1 - 1
                          ? brams0[i2][i1 + 1][head0]
                          : (i2 < W_EXTENT2 - 1 ? brams1[i2 + 1][head1] : 0)));
        }
      }
    }

    // self-peeled implementation
    //         for (int i2 = 0; i2 < W_EXTENT2-1; i2++) {
    // #pragma HLS unroll
    //             brams1[i2][tail1] = brams0[i2][0][head0];
    //             for (int i1 = 0; i1 < W_EXTENT1-1; i1++) {
    // #pragma HLS unroll
    //                 brams0[i2][i1][tail0] = regs[i2][i1][0];
    //                 for (int i0 = 0; i0 < W_EXTENT0-1; i0++) {
    // #pragma HLS unroll
    //                     regs[i2][i1][i0] = (i0 < W_EXTENT0-1 ?
    //                     regs[i2][i1][i0+1] :
    //                                         (i1 < W_EXTENT1-1 ?
    //                                         brams0[i2][i1+1][head0] :
    //                                          (i2 < W_EXTENT2-1 ?
    //                                          brams1[i2+1][head1] : 0)));
    //                 }
    //                 regs[i2][i1][W_EXTENT0-1] = brams0[i2][i1+1][head0];
    //             }
    //             // peel i1
    //             {
    //                 const int i1 = W_EXTENT1-1;
    //                 brams0[i2][i1][tail0] = regs[i2][i1][0];
    //                 for (int i0 = 0; i0 < W_EXTENT0-1; i0++) {
    // #pragma HLS unroll
    //                     regs[i2][i1][i0] = (i0 < W_EXTENT0-1 ?
    //                     regs[i2][i1][i0+1] :
    //                                         (i1 < W_EXTENT1-1 ?
    //                                         brams0[i2][i1+1][head0] :
    //                                          (i2 < W_EXTENT2-1 ?
    //                                          brams1[i2+1][head1] : 0)));
    //                 }
    //                 regs[i2][i1][W_EXTENT0-1] = brams1[i2+1][head1];
    //             }
    //         }
    //         // peel i2
    //         {
    //             const int i2 = W_EXTENT2-1;
    //             brams1[i2][tail1] = brams0[i2][0][head0];
    //             for (int i1 = 0; i1 < W_EXTENT1-1; i1++) {
    // #pragma HLS unroll
    //                 brams0[i2][i1][tail0] = regs[i2][i1][0];
    //                 for (int i0 = 0; i0 < W_EXTENT0-1; i0++) {
    // #pragma HLS unroll
    //                     regs[i2][i1][i0] = (i0 < W_EXTENT0-1 ?
    //                     regs[i2][i1][i0+1] :
    //                                         (i1 < W_EXTENT1-1 ?
    //                                         brams0[i2][i1+1][head0] :
    //                                          (i2 < W_EXTENT2-1 ?
    //                                          brams1[i2+1][head1] : 0)));
    //                 }
    //                 regs[i2][i1][W_EXTENT0-1] = brams0[i2][i1+1][head0];
    //             }
    //             // peel i1
    //             {
    //                 const int i1 = W_EXTENT1-1;
    //                 brams0[i2][i1][tail0] = regs[i2][i1][0];
    //                 for (int i0 = 0; i0 < W_EXTENT0-1; i0++) {
    // #pragma HLS unroll
    //                     regs[i2][i1][i0] = (i0 < W_EXTENT0-1 ?
    //                     regs[i2][i1][i0+1] :
    //                                         (i1 < W_EXTENT1-1 ?
    //                                         brams0[i2][i1+1][head0] :
    //                                          (i2 < W_EXTENT2-1 ?
    //                                          brams1[i2+1][head1] : 0)));
    //                 }
    //             }
    //         }

    increment_ptrs();
  }

  void rotate_d1() {
#pragma HLS inline
#pragma HLS dependence variable = brams0 intra false
#pragma HLS dependence variable = brams0 inter RAW distance = 2 true
#pragma HLS dependence variable = brams1 intra false
#pragma HLS dependence variable = brams1 inter RAW distance = 2 true
    for (int i2 = 0; i2 < W_EXTENT2; i2++) {
#pragma HLS unroll
      for (int i1 = 0; i1 < W_EXTENT1; i1++) {
#pragma HLS unroll
        brams0[i2][i1][tail0] = regs[i2][i1][0];
        for (int i0 = 0; i0 < W_EXTENT0; i0++) {
#pragma HLS unroll
          regs[i2][i1][i0] =
              i0 < W_EXTENT0 - 1 ? regs[i2][i1][i0 + 1] : brams0[i2][i1][head0];
        }
      }
    }

    increment_ptrs_d0();
  }

  void rotate_d2() {
#pragma HLS inline
#pragma HLS dependence variable = brams0 intra false
#pragma HLS dependence variable = brams0 inter RAW distance = 2 true
#pragma HLS dependence variable = brams1 intra false
#pragma HLS dependence variable = brams1 inter RAW distance = 2 true
    for (int i2 = 0; i2 < W_EXTENT2; i2++) {
#pragma HLS unroll
      brams1[i2][tail1] = brams0[i2][0][head0];
      for (int i1 = 0; i1 < W_EXTENT1; i1++) {
#pragma HLS unroll
        brams0[i2][i1][tail0] = regs[i2][i1][0];
        for (int i0 = 0; i0 < W_EXTENT0; i0++) {
#pragma HLS unroll
          regs[i2][i1][i0] =
              (i0 < W_EXTENT0 - 1
                   ? regs[i2][i1][i0 + 1]
                   : (i1 < W_EXTENT1 - 1 ? brams0[i2][i1 + 1][head0]
                                         : brams1[i2][head1]));
        }
      }
    }

    increment_ptrs();
  }

  void store(T v, int32_t i0, int32_t i1, int32_t i2) {
#pragma HLS inline
    regs[i2][i1][i0] = v;
  }

  void store(T v) {
#pragma HLS inline
    store(v, W_EXTENT0 - 1, W_EXTENT1 - 1, W_EXTENT2 - 1);
  }

  T load(int32_t i0, int32_t i1, int32_t i2) const {
#pragma HLS inline
    return regs[i2][i1][i0];
  }

  T load() const {
#pragma HLS inline
    return load(W_EXTENT0 - 1, W_EXTENT1 - 1, W_EXTENT2 - 1);
  }

  void print() const {
    const int32_t bram0_extent = (extent0 - W_EXTENT0) * 1;
    const int32_t bram1_extent = (extent1 - W_EXTENT1) * extent0;
    ;

    for (int i2 = 0; i2 < W_EXTENT2; i2++) {
      {
        std::cout << "{{";
        for (int i = 0; i < bram1_extent; i++) {
          const int bram1_i = (i + head1) % BRAM1_EXTENT;
          std::cout << std::setw(3)
                    << static_cast<int64_t>(brams1[i2][bram1_i]);
          if (i < bram1_extent - 1) {
            std::cout << " ,  ";
            if (i % extent0 == extent0 - 1) {
              std::cout << std::endl;
            }
          }
        }
        std::cout << "}}, " << std::endl;
      }
      {
        for (int i1 = 0; i1 < W_EXTENT1; i1++) {
          std::cout << " {";
          for (int i = 0; i < bram0_extent; i++) {
            const int bram0_i = (i + head0) % BRAM0_EXTENT;
            std::cout << std::setw(3)
                      << static_cast<int64_t>(brams0[i2][i1][bram0_i]);
            if (i < bram0_extent - 1) {
              std::cout << " ,  ";
            }
          }
          std::cout << "}, [";
          for (int i0 = 0; i0 < W_EXTENT0; i0++) {
            std::cout << std::setw(3) << static_cast<int64_t>(regs[i2][i1][i0]);
            if (i0 < W_EXTENT0 - 1) {
              std::cout << " ,  ";
            }
          }
          std::cout << "] ";
          if (i1 < W_EXTENT1 - 1) {
            std::cout << ",";
          }
          std::cout << std::endl;
        }
      }
    }
    std::cout << std::endl;
  }
};

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_LINEBUFFER_H
