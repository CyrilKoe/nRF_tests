#!/bin/bash

# Generate project inside ./build folder.
cmake \
    -S . \
    -B build \
    -DCMAKE_TOOLCHAIN_FILE="./cmake/arm-none-eabi.cmake" \
    -DTOOLCHAIN_PREFIX="/home/ckoenig/Documents/wrlds/gnuarmemb" \
    -DNRF5_SDK_PATH="/home/ckoenig/Documents/wrlds/tests_ml/cmake-nRF5x/ci/sdks/15.3.0" \
    -DNRF5_BOARD="pca10040" \
    -DNRF5_SOFTDEVICE_VARIANT="s132"
