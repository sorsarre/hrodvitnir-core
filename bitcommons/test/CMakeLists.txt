cmake_minimum_required(VERSION 3.12)

enable_testing()

############## Commons gtest
add_executable(bitcommons_gtest
        bitreader_gtest.cpp
        bitwriter_gtest.cpp
        file_byte_source_gtest.cpp
        gtest_common.hpp
        gtest_common_gtest.cpp
        memory_byte_source_gtest.cpp
        shared_buffer_gtest.cpp
        )

target_include_directories(bitcommons_gtest PRIVATE ${GTEST_INCLUDE_DIRS})

target_link_libraries(bitcommons_gtest bitcommonscpp)
target_link_libraries(bitcommons_gtest ${GTEST_BOTH_LIBRARIES})
target_link_libraries(bitcommons_gtest pthread)
set_target_properties(bitcommons_gtest PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

gtest_discover_tests(bitcommons_gtest)

install(TARGETS bitcommons_gtest RUNTIME DESTINATION test)
