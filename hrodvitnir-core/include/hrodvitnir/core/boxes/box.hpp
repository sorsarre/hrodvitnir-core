// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>
#include <hrodvitnir/core/uuid.hpp>

namespace hrodvitnir::core::boxes
{
template <typename Base>
struct box : public Base
{
    MUCH_BLACKER_MAGICK(__pos, r_manual<uint64_t>);
    MUCH_BLACKER_MAGICK(__size, r_manual<uint64_t>);
    MUCH_BLACKER_MAGICK(size, r_uint<32>);
    MUCH_BLACKER_MAGICK(type, r_fourcc);
    MUCH_BLACKER_MAGICK(largesize, r_uint<64>);
    MUCH_BLACKER_MAGICK(uuid, r_uuid);

    [[nodiscard]] uint64_t box_size() const
    {
        return __size;
    }

    [[nodiscard]] uint64_t box_pos() const
    {
        return __pos;
    }

    [[nodiscard]] uint64_t box_end() const
    {
        return __pos() + __size();
    }

    template <typename Reader>
    void read_basic(Reader& r)
    {
        __pos = r.position_bytes();
        size << r;
        type << r;
        if (size == 1)
        {
            largesize << r;
            __size = largesize;
        }
        else if (size == 0)
        {
            largesize = r.available_bytes();
            __size = largesize;
        }
        else
        {
            __size = size;
        }

        if (type() == fourcc("uuid"))
        {
            uuid << r;
        }
        else
        {
            uuid = uuid::from_fourcc(type);
        }
    }

    template <typename Reader>
    void read(Reader& r)
    {
        // nope, at least until I find a better way to get around that
    }
};
} // namespace hrodvitnir::core::boxes
