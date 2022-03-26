// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include "any_printer.hpp"

#include <bitcommons/bitreader.hpp>
#include <bitcommons/direct_file_reader.hpp>
#include <bitcommons/file_byte_source.hpp>
#include <hrodvitnir/core/data-source-wrapper.hpp>
#include <hrodvitnir/core/parser/box-parser.hpp>
#include <hrodvitnir/core/parser/default-mapping.hpp>
#include <hrodvitnir/core/tree/tree-builder.hpp>
#include <iostream>

using namespace hrodvitnir;
namespace brcpp = bitcommons;

int main(int argc, char** argv)
{
    if (argc < 2)
    {
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
