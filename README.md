# Vivado HLS Test
A project for test and evaluate the program written by Vivado HLS C/C++.

## Build
#### Clone
```sh
git clone https://github.com/akmaru/vivado-hls-test.git
```

#### Build
If Vivado HLS root directory is "/opt/Xilinx/Vivado/2019.1", execute the following commands.

```sh
mkdir build
cmake -DCMAKE_BUILD_TYPE=Debug -DVIVADO_HLS_ROOT=/opt/Xilinx/Vivado/2019.1 ../
make
```

## Test
If you want to run the test of `copy`, execute the following commands.

#### C simulation test
```sh
ctest -V -R "copy_csim"
```

#### C/RTL co-simulation test
```sh
ctest -V -R "copy_cosim"

```
## Add the test
Please reference `tests/copy` directory.
