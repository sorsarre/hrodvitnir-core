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

add_library(hrodvitnir-core INTERFACE)
target_include_directories(hrodvitnir-core INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

install(TARGETS hrodvitnir-core
        EXPORT hrodvitnir-core-export
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        )

install(DIRECTORY include/hrodvitnir DESTINATION include)
install(EXPORT hrodvitnir-core-export DESTINATION cmake NAMESPACE Hrodvitnir::)