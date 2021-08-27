#
# Copyright (c) 2019 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test_fsm_2)

# generate runner for the test
test_runner_generate(src/test.c)

# create a mock for leds
cmock_handle(../../../ncs/nrf/include/dk_buttons_and_leds.h)

# add module fsm
target_sources(app PRIVATE ../../app/src/fsm.c)
target_include_directories(app PRIVATE ../../app/src)

# add test file
target_sources(app PRIVATE src/test.c)
target_include_directories(app PRIVATE .)
