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
struct sample_to_chunk_entry
{
    uint32_t first_chunk;
    uint32_t samples_per_chunk;
    uint32_t sample_description_index;
};

template <typename Base>
struct sample_to_chunk : public Base
{
    using r_stsc_entry = r_lambda < [](auto& r) -> auto
    {
        sample_to_chunk_entry ret;
        using r_uint32 = r_uint<32>;
        ret.first_chunk = r_uint32::read(r);
        ret.samples_per_chunk = r_uint32::read(r);
        ret.sample_description_index = r_uint32::read(r);
        return ret;
    }
    > ;

    MUCH_BLACKER_MAGICK(_contents, r_array_table<r_stsc_entry, r_uint<32>>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        _contents << r;
    }
};
} // namespace hrodvitnir::core::boxes
