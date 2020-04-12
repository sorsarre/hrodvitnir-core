/*
BSD 3-Clause License

Copyright (c) 2019-2020, Andrey Kaydalov
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <iostream>

#include <bitcommons/bitreader.hpp>
#include <bitcommons/direct_file_reader.hpp>
#include <bitcommons/file_byte_source.hpp>

#include <hrodvitnir/core/parser/box-parser.hpp>
#include <hrodvitnir/core/data-source-wrapper.hpp>
#include <hrodvitnir/core/tree/tree-builder.hpp>
#include <hrodvitnir/core/parser/default-mapping.hpp>

#include "any_printer.hpp"

using namespace hrodvitnir;
namespace brcpp = bitcommons;


int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "[ERROR] too few arguments, please supply file name" << std::endl;
        return 1;
    }

    auto file_reader = brcpp::direct_file_reader::open(argv[1]);
    auto source_raw = std::make_shared<brcpp::file_byte_source>(file_reader);
    auto source_wrapped = core::wrap_data_source(source_raw);
    core::reader_type reader{source_wrapped};

    const auto mapping = core::get_base_mapping();
    auto builder = std::make_shared<core::tree_builder>();

    core::box_parser parser;
    parser.parse(reader, mapping, builder);

    auto tree = builder->get();
    dump::dump_context ctx{2, std::cout};
    dump::print_tree(ctx, tree);
    return 0;
}
