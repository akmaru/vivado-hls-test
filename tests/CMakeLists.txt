include(${VHLS_TEST_CMAKE_DIRS}/module.cmake)

set(TB_TCL ${CMAKE_CURRENT_SOURCE_DIR}/tb.tcl)

set(modules
  copy
  vecadd
)

foreach(m ${modules})
  add_subdirectory(${m})
endforeach()
