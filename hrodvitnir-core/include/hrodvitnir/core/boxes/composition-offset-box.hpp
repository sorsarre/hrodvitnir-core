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
struct composition_offset_entry
{
    uint32_t sample_count;
    int32_t sample_offset;
};

template <typename Base>
struct composition_offset : public Base
{
    using r_ctts_entry = r_lambda < [](auto& r) -> auto
    {
        composition_offset_entry ret;
        ret.sample_count = r_uint<32>::read(r);
        ret.sample_offset = r_int<32>::read(r);
        return ret;
    }
    > ;

    MUCH_BLACKER_MAGICK(_contents, r_array_table<r_ctts_entry, r_uint<32>>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        _contents << r;
    }
};
} // namespace hrodvitnir::core::boxes
