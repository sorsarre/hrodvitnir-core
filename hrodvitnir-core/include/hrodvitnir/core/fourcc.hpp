// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <cstddef>
#include <cstdint>
#include <string>

namespace hrodvitnir::core
{
class fourcc
{
  public:
    //----------------------------------------------------------------------
    constexpr fourcc() : _data(0){};
    constexpr fourcc(uint32_t value) : _data(value) {}
    constexpr fourcc(const char* str, size_t sz = 4) : _data(from_string(str, sz)) {}

    //----------------------------------------------------------------------
    constexpr bool operator!=(const fourcc& other) const
    {
        return other._data != _data;
    }

    //----------------------------------------------------------------------
    constexpr bool operator==(const fourcc& other) const
    {
        return other._data == _data;
    }

    //----------------------------------------------------------------------
    constexpr bool operator<(const fourcc& other) const
    {
        return _data < other._data;
    }

    //----------------------------------------------------------------------
    constexpr bool operator>(const fourcc& other) const
    {
        return _data > other._data;
    }

    //----------------------------------------------------------------------
    std::string string() const;

    //----------------------------------------------------------------------
    constexpr uint32_t value() const
    {
        return _data;
    }

  private:
    //----------------------------------------------------------------------
    static constexpr uint32_t from_string(const char* s, size_t sz)
    {
        uint32_t ret = 0;
        for (size_t iter = 0; iter < sz; ++iter)
        {
            ret <<= 8;
            ret |= s[iter];
        }
        return ret;
    }

    uint32_t _data;
};
} // namespace hrodvitnir::core
