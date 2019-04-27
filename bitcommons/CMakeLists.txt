cmake_minimum_required(VERSION 3.12)


set(BITCOMMONS_SOURCES
        src/direct_file_reader.cpp
        src/file_byte_source.cpp
        src/memory_byte_source.cpp
        src/shared_buffer.cpp)

set(BITCOMMONS_HEADERS
        include/bitcommons/abstract_byte_source.hpp
        include/bitcommons/bitreader.hpp
        include/bitcommons/bitreader-utils.hpp
        include/bitcommons/bitwriter.hpp
        include/bitcommons/direct_file_reader.hpp
        include/bitcommons/file_byte_source.hpp
        include/bitcommons/file_reader.hpp
        include/bitcommons/memory_byte_source.hpp
        include/bitcommons/shared_buffer.hpp
    )

add_library(bitcommonscpp STATIC ${BITCOMMONS_SOURCES} ${BITCOMMONS_HEADERS})
target_include_directories(bitcommonscpp PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)
target_include_directories(bitcommonscpp PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:include/bitcommons>
        )
set_target_properties(bitcommonscpp PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
)

if (NOT WIN32)
    set_target_properties(bitcommonscpp PROPERTIES
        COMPILE_FLAGS "-Wall -Werror -pedantic"
    )
endif()

enable_testing()
find_package(GTest)

if (NOT GTEST_FOUND)
    message(WARNING "Unit tests are disabled - gtest not found")
else()
    add_subdirectory(test)
endif()

install(TARGETS bitcommonscpp
        EXPORT bitcommonscpp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY include/bitcommons DESTINATION include)
install(EXPORT bitcommonscpp DESTINATION cmake NAMESPACE Hrodvitnir::)