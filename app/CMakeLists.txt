#
# Copyright (c) 2018 Nordic Semiconductor
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#
cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/fsm.c)
target_include_directories(app PRIVATE src)


# NORDIC SDK APP END
zephyr_library_include_directories(.)
