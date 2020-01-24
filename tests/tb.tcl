set mode [lindex $argv 2]
set name [lindex $argv 3]
set hls_srcs [lindex $argv 4]
set top [lindex $argv 5]
set include_paths [lindex $argv 6]
set cxxflags [lindex $argv 7]
set test_srcs [lindex $argv 8]
set test_args [lindex $argv 9]

open_project -reset ${name}

set test_cxxflags "-I ${include_paths}"
regsub "cxxflags=" $cxxflags {} cxxflags

set_top ${top}
add_files ${hls_srcs} -cflags "${test_cxxflags} ${cxxflags}"

open_solution "solution1"
set_part {xczu3eg-sfvc784-2L-e}
create_clock -period 5.00 -name default

csynth_design

if {${mode} == "cosim"} {
    add_files -tb ${test_srcs} -cflags "${test_cxxflags} ${cxxflags}"
    cosim_design -compiler gcc -ldflags "-ldl" -trace_level port -argv "${test_args}"
}

if {${mode} == "impl"} {
    export_design -flow impl -rtl verilog -format ip_catalog
}

exit
