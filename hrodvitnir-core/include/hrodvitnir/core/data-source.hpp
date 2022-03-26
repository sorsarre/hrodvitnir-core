// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>

namespace hrodvitnir::core
{
// TODO: Move it over into bitreader
class data_source
{
  public:
    virtual size_t get_n(uint64_t& buf, size_t bytes) = 0;
    virtual bool depleted() = 0;
    virtual uint64_t available() = 0;
    virtual uint64_t position() = 0;
    virtual void seek(uint64_t position) = 0;
    virtual void skip(uint64_t bytes) = 0;
    virtual std::shared_ptr<data_source> clone() = 0;

    virtual ~data_source() = default;
};
} // namespace hrodvitnir::core
