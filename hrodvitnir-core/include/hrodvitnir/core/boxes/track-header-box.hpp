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
struct track_header : public Base
{
    MUCH_BLACKER_MAGICK(creation_time, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(modification_time, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(track_ID, r_uint<32>);
    MUCH_BLACKER_MAGICK(duration, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(layer, r_uint<16>);
    MUCH_BLACKER_MAGICK(alternate_group, r_uint<16>);
    MUCH_BLACKER_MAGICK(volume, r_qmn<uint16_t, 8>);
    MUCH_BLACKER_MAGICK(matrix, r_transform);
    MUCH_BLACKER_MAGICK(width, r_qmn<uint32_t, 16>);
    MUCH_BLACKER_MAGICK(height, r_qmn<uint32_t, 16>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        size_t bitcount = (Base::version == 1) ? 64 : 32;
        creation_time << read_args(r, bitcount);
        ;
        modification_time << read_args(r, bitcount);
        ;
        track_ID << r;
        r.skip(32); // reserved
        duration << read_args(r, bitcount);
        ;

        r.skip(32 * 2); // reserved

        layer << r;
        alternate_group << r;
        volume << r;

        r.skip(16); // reserved

        matrix << r;
        width << r;
        height << r;
    }
};
} // namespace hrodvitnir::core::boxes
