// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <algorithm>
#include <cstdint>
#include <hrodvitnir/core/fourcc.hpp>

namespace hrodvitnir::core
{
static constexpr const uint8_t ISO_12_reserved[] = {0x00, 0x11, 0x00, 0x10, 0x80, 0x00,
                                                    0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};

class uuid
{
  public:
    //----------------------------------------------------------------------
    bool is_iso() const;
    static uuid from_fourcc(const fourcc& fcc);
    static uuid from_fourcc(uint32_t fourcc);
    std::string to_string() const;
    bool operator<(const uuid& other) const;
    bool operator==(const uuid& other) const;
    bool operator!=(const uuid& other) const;

    const uint8_t* begin() const
    {
        return data;
    }
    const uint8_t* end() const
    {
        return data + 16;
    }
    uint8_t* begin()
    {
        return data;
    }
    uint8_t* end()
    {
        return data;
    }

    template <typename ForwardIterator, typename Iterator>
    static uuid from_bytes(ForwardIterator begin, Iterator end)
    {
        uuid ret;
        std::copy(begin, end, ret.data);
        return ret;
    }

  private:
    void convert_partial(std::string& target, size_t begin, size_t end) const;

    uint8_t data[16] = {0};
};
} // namespace hrodvitnir::core
