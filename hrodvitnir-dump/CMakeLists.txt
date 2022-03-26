# SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.12)
project(hrodvitnir-dump)

set(HRODVITNIR_DUMP_SRC src/main.cpp src/any_printer.hpp)
add_executable(hrodvitnir-dump ${HRODVITNIR_DUMP_SRC})
set_target_properties(hrodvitnir-dump PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
)
target_link_libraries(hrodvitnir-dump hrodvitnir-core bitcommonscpp)

if (NOT WIN32)
    set_target_properties(hrodvitnir-dump PROPERTIES
        COMPILE_FLAGS "-Wall -pedantic"
    )
endif()

install(TARGETS hrodvitnir-dump
    RUNTIME DESTINATION bin
)
