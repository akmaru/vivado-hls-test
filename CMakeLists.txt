cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
enable_testing()

# Project
set(PROJECT_NAME vivado-hls-test)
project(${PROJECT_NAME} LANGUAGES C CXX)

# Default to Debug build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Project settings
set(VHLS_TEST_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/tests)
set(VHLS_TEST_TEST_DIR ${PROJECT_SOURCE_DIR}/tests)
set(VHLS_TEST_CXX_FLAGS "-Wall" "-Wno-unused-function" "-Wno-unknown-pragmas")
set(VHLS_TEST_CMAKE_DIRS ${PROJECT_SOURCE_DIR}/cmake)

# Vivado HLS
set(VIVADO_HLS_ROOT "/opt/Xilinx/Vitis/2019.2" CACHE STRING "Path to Vivado HLS root directory")
set(VIVADO_HLS_INCLUDE_DIRS ${VIVADO_HLS_ROOT}/include)

# Logging
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Vivado HLS path: ${VIVADO_HLS_ROOT}")

add_subdirectory(tests)

# Test
add_custom_target(csim COMMAND "ctest" "-R" "_csim$")
add_custom_target(cosim COMMAND "ctest" "-R" "_cosim$")
