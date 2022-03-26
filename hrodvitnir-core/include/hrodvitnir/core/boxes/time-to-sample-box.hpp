// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>
#include <hrodvitnir/core/table.hpp>

namespace hrodvitnir::core::boxes
{
struct time_to_sample_entry
{
    uint32_t sample_count;
    uint32_t sample_delta;
};

template <typename Base>
struct time_to_sample : public Base
{
    using r_stts_entry = r_lambda < [](auto& r) -> auto
    {
        time_to_sample_entry ret;
        ret.sample_count = r_uint<32>::read(r);
        ret.sample_delta = r_uint<32>::read(r);
        return ret;
    }
    > ;

    MUCH_BLACKER_MAGICK(_contents, r_array_table<r_stts_entry, r_uint<32>>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        _contents << r;
    }
};
} // namespace hrodvitnir::core::boxes
