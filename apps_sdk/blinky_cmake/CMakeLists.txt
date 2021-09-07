cmake_minimum_required(VERSION 3.14)
project(blinky LANGUAGES C ASM)

set(PROJECT_FOLDER ${PROJECT_SOURCE_DIR})

set(NRF5_SDKCONFIG_PATH ${PROJECT_FOLDER}/pca10040/s132/config/)
set(NRF5_LINKER_SCRIPT ${PROJECT_FOLDER}/pca10040/s132/armgcc/blinky_gcc_nrf52.ld)


include("nrf5")

add_executable(${CMAKE_PROJECT_NAME}
  "src/main.c"
)
nrf5_target(${CMAKE_PROJECT_NAME})
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
  nrf5_nrfx_common  
  nrf5_delay
  nrf5_boards
)
