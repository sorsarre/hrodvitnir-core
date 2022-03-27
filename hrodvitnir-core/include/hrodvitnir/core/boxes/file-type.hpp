// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>

namespace hrodvitnir::core::boxes
{
template <typename Base>
struct file_type : public Base
{
    MUCH_BLACKER_MAGICK(major_brand, r_fourcc);
    MUCH_BLACKER_MAGICK(minor_version, r_uint<32>);
    MUCH_BLACKER_MAGICK(compatible_brands, r_items_arg<std::vector, r_fourcc>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        major_brand << r;
        minor_version << r;

        size_t count = (Base::box_end() - r.position_bytes()) / 4;
        compatible_brands << read_args(r, count);
    }
};
} // namespace hrodvitnir::core::boxes
