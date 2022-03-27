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
struct visual_sample_entry : public Base
{
    MUCH_BLACKER_MAGICK(width, r_uint<16>);
    MUCH_BLACKER_MAGICK(height, r_uint<16>);
    MUCH_BLACKER_MAGICK(horizresolution, r_qmn<uint32_t, 16>);
    MUCH_BLACKER_MAGICK(vertresolution, r_qmn<uint32_t, 16>);
    MUCH_BLACKER_MAGICK(frame_count, r_uint<16>);
    MUCH_BLACKER_MAGICK(compressorname, r_array<32, r_char<8>>);
    MUCH_BLACKER_MAGICK(depth, r_uint<16>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        r.skip_bytes(16); // various reserved fields
        width << r;
        height << r;
        horizresolution << r;
        vertresolution << r;
        r.skip_bytes(4); // reserved
        frame_count << r;
        compressorname << r;
        depth << r;
        r.skip_bytes(2); // predefined
    }
};
} // namespace hrodvitnir::core::boxes
