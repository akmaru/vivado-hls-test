include(CMakeParseArguments)

macro(abs_path files abs_paths)
  foreach(f ${files})
    list(APPEND ${abs_paths} ${CMAKE_CURRENT_SOURCE_DIR}/${f})
  endforeach()
endmacro()

macro(list2str l str)
  string(REPLACE ";" " " ${str} "${l}")
endmacro()


function(add_csim name)
  cmake_parse_arguments(ARG "" "" "HLS_SRC;TB_SRC;CXXFLAGS" ${ARGN})

  add_executable(${name} ${ARG_HLS_SRC} ${ARG_TB_SRC})

  target_include_directories(${name} PRIVATE ${VHLS_TEST_TEST_DIR} ${VHLS_TEST_INCLUDE_DIR} ${VIVADO_HLS_INCLUDE_DIRS})
  target_compile_options(${name} PRIVATE ${VHLS_TEST_CXX_FLAGS} ${ARG_CXXFLAGS})
  set_target_properties(${name} PROPERTIES
    CXX_STANDARD 98
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF)

  add_test(
    NAME ${name}
    COMMAND ${name} ${ARG_UNPARSED_ARGUMENTS}
    WORKING_DIRECTORY ${WORK_DIR}
    )

endfunction()

function(add_cosim name top)
  cmake_parse_arguments(ARG "" "" "HLS_SRC;TB_SRC;CXXFLAGS" ${ARGN})

  abs_path(${ARG_HLS_SRC} abs_hls_srcs)
  abs_path(${ARG_TB_SRC} abs_tb_srcs)
  list2str("${ARG_CXXFLAGS}" cxxflags_str)

  add_test(
    NAME ${name}
    COMMAND vivado_hls -f ${TB_TCL} "cosim" ${name} "${abs_hls_srcs}" ${top} ${VHLS_TEST_INCLUDE_DIR} "cxxflags=${cxxflags_str}" "${abs_tb_srcs}" "${ARG_UNPARSED_ARGUMENTS}"
    WORKING_DIRECTORY ${WORK_DIR}
    )
endfunction()

function(add_impl name top)
  cmake_parse_arguments(ARG "" "" "HLS_SRC;CXXFLAGS" ${ARGN})

  abs_path(${ARG_HLS_SRC} abs_hls_srcs)
  list2str("${ARG_CXXFLAGS}" cxxflags_str)

  add_custom_target(
    ${name}
    COMMAND vivado_hls -f ${TB_TCL} "impl" ${name} "${abs_hls_srcs}" ${top} ${VHLS_TEST_INCLUDE_DIR} "cxxflags=${cxxflags_str}" ${ARG_UNPARSED_ARGUMENTS}
    WORKING_DIRECTORY ${WORK_DIR}
    )
endfunction()

function(add_test_and_impl name top)
  cmake_parse_arguments(ARG "" "" "HLS_SRC;TB_SRC;CXXFLAGS" ${ARGN})

  add_csim(${name}_csim CXXFLAGS ${ARG_CXXFLAGS} HLS_SRC ${ARG_HLS_SRC} TB_SRC ${ARG_TB_SRC})
  add_cosim(${name}_cosim ${top} CXXFLAGS ${ARG_CXXFLAGS} HLS_SRC ${ARG_HLS_SRC} TB_SRC ${ARG_TB_SRC})
  add_impl(${name}_impl ${top} CXXFLAGS ${ARG_CXXFLAGS} HLS_SRC ${ARG_HLS_SRC})
endfunction()
