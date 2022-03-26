# SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.15)

enable_testing()

########### Core tests
add_executable(hrodvitnir-core-gtest
    fourcc-gtest.cpp
)

target_include_directories(hrodvitnir-core-gtest PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(hrodvitnir-core-gtest bitcommonscpp)
target_link_libraries(hrodvitnir-core-gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(hrodvitnir-core-gtest pthread)
target_link_libraries(hrodvitnir-core-gtest hrodvitnir-core)
set_target_properties(hrodvitnir-core-gtest PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON
)

gtest_discover_tests(hrodvitnir-core-gtest)

install(TARGETS hrodvitnir-core-gtest RUNTIME DESTINATION test)
