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
struct movie_header : public Base
{
    MUCH_BLACKER_MAGICK(creation_time, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(modification_time, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(timescale, r_uint<32>);
    MUCH_BLACKER_MAGICK(duration, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(rate, r_qmn<int32_t, 16>);
    MUCH_BLACKER_MAGICK(volume, r_qmn<int16_t, 8>);
    MUCH_BLACKER_MAGICK(matrix, r_transform);
    MUCH_BLACKER_MAGICK(next_track_ID, r_uint<32>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        size_t bitcount = (Base::version == 1) ? 64 : 32;
        creation_time << read_args(r, bitcount);
        modification_time << read_args(r, bitcount);
        timescale << r;
        duration << read_args(r, bitcount);
        rate << r;
        volume << r;
        r.skip_bytes(10); // reserved
        matrix << r;
        r.skip_bytes(24); // reserved
        next_track_ID << r;
    }
};
} // namespace hrodvitnir::core::boxes
