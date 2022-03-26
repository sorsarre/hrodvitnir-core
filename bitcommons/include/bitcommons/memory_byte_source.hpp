// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/shared_buffer.hpp>
#include <stdexcept>
#include <string>

namespace bitcommons
{
class memory_byte_source
{
  public:
    memory_byte_source();
    memory_byte_source(const uint8_t* data, size_t size);
    size_t get_n(uint64_t& buf, size_t bytes);
    bool depleted();
    uint64_t available();
    uint64_t position();
    void seek(uint64_t position);
    void skip(uint64_t bytes);
    std::shared_ptr<memory_byte_source> clone();

  private:
    shared_buffer::iterator _current;
    shared_buffer _data;
};
} // namespace bitcommons
