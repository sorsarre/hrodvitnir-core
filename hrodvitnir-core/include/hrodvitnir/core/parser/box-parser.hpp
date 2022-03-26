// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-consumer.hpp>
#include <hrodvitnir/core/box-defs.hpp>
#include <hrodvitnir/core/parser/reader-mapping.hpp>
#include <memory>
#include <stack>

namespace hrodvitnir::core
{
class box_parser
{
  public:
    void parse(reader_type& reader, reader_mapping::ptr mapping, box_consumer::ptr consumer);
};
} // namespace hrodvitnir::core
