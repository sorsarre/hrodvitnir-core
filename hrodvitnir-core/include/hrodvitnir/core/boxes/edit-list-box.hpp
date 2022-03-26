// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>

namespace hrodvitnir::core::boxes
{
struct edit_list_item
{
    uint64_t segment_duration;
    int64_t media_time;
    int16_t media_rate_integer; // TODO: replace with Q16.16
    int16_t media_rate_fraction;
};

struct r_elst_item
{
    using value_type = edit_list_item;

    template <typename Reader>
    static value_type read(Reader& r, uint32_t version)
    {
        value_type ret;
        size_t bitcount = (version == 1) ? 64 : 32;
        r.read(bitcount, ret.segment_duration);
        r.read(bitcount, ret.media_time);
        r.read(16, ret.media_rate_integer);
        r.read(16, ret.media_rate_fraction);
        return ret;
    }
};

template <typename Base>
struct edit_list : public Base
{
    MUCH_BLACKER_MAGICK(entry_count, r_uint<32>);
    MUCH_BLACKER_MAGICK(entries, r_arg_items_arg<std::vector, r_elst_item>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        entry_count << r;
        entries << read_args(r, entry_count(), Base::version());
    }
};
} // namespace hrodvitnir::core::boxes
