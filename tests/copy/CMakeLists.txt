set(hls_src copy_hls.cc)
set(test_src copy_test.cc)

add_test_and_impl(copy copy_hls HLS_SRC ${hls_src} TB_SRC ${test_src})
