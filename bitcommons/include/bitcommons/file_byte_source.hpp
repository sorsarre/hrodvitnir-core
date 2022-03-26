// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/file_reader.hpp>
#include <bitcommons/shared_buffer.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>

namespace bitcommons
{
class file_byte_source
{
  public:
    explicit file_byte_source(std::shared_ptr<file_reader> reader);
    size_t get_n(uint64_t& buf, size_t bytes);
    bool depleted();
    uint64_t available();
    uint64_t position();
    void seek(uint64_t position);
    void skip(uint64_t bytes);
    std::shared_ptr<file_byte_source> clone();

  private:
    void load_buffer();

    std::shared_ptr<file_reader> _reader;
    shared_buffer _buffer;
    uint64_t _position;
    uint64_t _last;
};
} // namespace bitcommons
