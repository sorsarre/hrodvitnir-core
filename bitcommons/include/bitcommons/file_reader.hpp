// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>

namespace bitcommons
{
class file_reader
{
  public:
    virtual size_t read(uint8_t* dest, uint64_t position, size_t bytes) = 0;
    virtual uint64_t size() = 0;
    virtual bool depleted() = 0;
    virtual std::shared_ptr<file_reader> clone() = 0;
    virtual ~file_reader() = default;
};
} // namespace bitcommons
