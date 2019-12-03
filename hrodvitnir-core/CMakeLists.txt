cmake_minimum_required(VERSION 3.12)
project(hrodvitnir-core)

set(HRODVITNIR_CORE_HEADERS
        include/hrodvitnir/core/bitreader-helpers.hpp
        include/hrodvitnir/core/box-binding.hpp
        include/hrodvitnir/core/box-consumer.hpp
        include/hrodvitnir/core/box-defs.hpp
        include/hrodvitnir/core/box-property.hpp
        include/hrodvitnir/core/ce_crc64.hpp
        include/hrodvitnir/core/data-source.hpp
        include/hrodvitnir/core/data-source-wrapper.hpp
        include/hrodvitnir/core/fieldset.hpp
        include/hrodvitnir/core/fourcc.hpp
        include/hrodvitnir/core/meta-helpers.hpp
        include/hrodvitnir/core/readspec.hpp
        include/hrodvitnir/core/uuid.hpp
        include/hrodvitnir/core/boxes/box.hpp
        include/hrodvitnir/core/boxes/file-type.hpp
        include/hrodvitnir/core/boxes/full-box.hpp
        include/hrodvitnir/core/boxes/skipper-box.hpp
        include/hrodvitnir/core/parser/box-parser.hpp
        include/hrodvitnir/core/parser/reader-mapping.hpp
        include/hrodvitnir/core/parser/simple-reader-mapping.hpp
        include/hrodvitnir/core/tree/tree.hpp
        include/hrodvitnir/core/tree/tree-builder.hpp
        include/hrodvitnir/core/tree/tree-node.hpp
)

set(HRODVITNIR_CORE_SOURCES
        src/tree.cpp
        src/tree-builder.cpp
        src/tree-node.cpp
        src/fieldset.cpp
        src/fourcc.cpp
        src/uuid.cpp
        src/box-parser.cpp
        src/simple-reader-mapping.cpp
        )

add_library(hrodvitnir-core STATIC ${HRODVITNIR_CORE_SOURCES} ${HRODVITNIR_CORE_HEADERS})
target_include_directories(hrodvitnir-core PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(hrodvitnir-core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_link_libraries(hrodvitnir-core bitcommonscpp)
set_target_properties(hrodvitnir-core PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED ON)


install(TARGETS hrodvitnir-core
        EXPORT hrodvitnir-core
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

enable_testing()
find_package(GTest)

if (NOT GTEST_FOUND)
        message(WARNING "Unit tests are disabled - gtest not found")
else()
        add_subdirectory(test)
endif()

install(DIRECTORY include/hrodvitnir DESTINATION include)
install(EXPORT hrodvitnir-core DESTINATION cmake NAMESPACE Hrodvitnir::)