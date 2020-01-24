set(hls_src vecadd_hls.cc)
set(test_src vecadd_test.cc)

add_test_and_impl(vecadd vecadd_hls HLS_SRC ${hls_src} TB_SRC ${test_src})
