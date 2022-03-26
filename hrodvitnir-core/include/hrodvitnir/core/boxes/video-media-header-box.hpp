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
struct video_media_header : public Base
{
    MUCH_BLACKER_MAGICK(graphicsmode, r_uint<16>);
    MUCH_BLACKER_MAGICK(opcolor, r_array<3, r_uint<16>>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        graphicsmode << r;
        opcolor << r;
    }
};
} // namespace hrodvitnir::core::boxes
