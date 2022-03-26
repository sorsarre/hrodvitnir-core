// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/bitreader.hpp>
#include <functional>
#include <hrodvitnir/core/data-source.hpp>
#include <hrodvitnir/core/fieldset.hpp>
#include <hrodvitnir/core/uuid.hpp>
#include <memory>

namespace hrodvitnir::core
{
using reader_type = bitcommons::bitreader<data_source>;
using partial_reader_type = std::function<void(fieldset&, reader_type&)>;

//--------------------------------------------------------------------------
class reader_mapping
{
  public:
    using ptr = std::shared_ptr<reader_mapping>;

    virtual const partial_reader_type& get(const uuid& u) const = 0;

    virtual ~reader_mapping() = default;
};
} // namespace hrodvitnir::core
