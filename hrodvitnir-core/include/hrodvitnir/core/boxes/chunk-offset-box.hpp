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

template <typename Base>
struct chunk_offset : public Base
{
    using r_stco_entry = r_uint<32>;

    MUCH_BLACKER_MAGICK(_contents, r_array_table<r_stco_entry, r_uint<32>>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        _contents << r;
    }
};
} // namespace hrodvitnir::core::boxes
