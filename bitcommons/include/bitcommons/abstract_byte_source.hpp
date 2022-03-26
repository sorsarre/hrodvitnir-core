// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/shared_buffer.hpp>
#include <memory>
#include <stdexcept>
#include <string>

namespace bitcommons
{
//--------------------------------------------------------------------------
class abstract_byte_source
{
  public:
    virtual size_t get_n(uint64_t& buf, size_t bytes) = 0;
    virtual bool depleted() = 0;
    virtual uint64_t available() = 0;
    virtual uint64_t position() = 0;
    virtual void seek(uint64_t position) = 0;
    virtual void skip(uint64_t bytes) = 0;
    virtual std::shared_ptr<abstract_byte_source> clone() = 0;

    virtual ~abstract_byte_source() = default;
};
} // namespace bitcommons
