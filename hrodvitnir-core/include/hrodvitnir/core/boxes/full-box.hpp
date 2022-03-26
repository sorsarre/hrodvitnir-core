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
struct full_box : public Base
{
    MUCH_BLACKER_MAGICK(version, r_uint<8>);
    MUCH_BLACKER_MAGICK(flags, r_bit<24>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        version << r;
        flags << r;
    }
};
} // namespace hrodvitnir::core::boxes
