cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

set(MODEL_FOLDER .)

target_sources(app PRIVATE src/fsm.c src/main.c)
target_include_directories(app PRIVATE src)

include(${MODEL_FOLDER}/edge-impulse-sdk/cmake/utils.cmake)
add_subdirectory(${MODEL_FOLDER}/edge-impulse-sdk/cmake/zephyr)

target_include_directories(app PRIVATE
    ${MODEL_FOLDER}
    ${MODEL_FOLDER}/tflite-model
    ${MODEL_FOLDER}/model-parameters
)
include_directories(${INCLUDES})

# find model source files
RECURSIVE_FIND_FILE(MODEL_FILES "${MODEL_FOLDER}/tflite-model" "*.cpp")
list(APPEND SOURCE_FILES ${MODEL_FILES})

# NORDIC SDK APP END
zephyr_library_include_directories(.)
