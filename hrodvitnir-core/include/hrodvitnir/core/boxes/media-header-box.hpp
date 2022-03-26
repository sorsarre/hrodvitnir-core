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
struct media_header : public Base
{
    MUCH_BLACKER_MAGICK(creation_time, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(modification_time, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(timescale, r_uint<32>);
    MUCH_BLACKER_MAGICK(duration, r_arg_bits<uint64_t>);
    MUCH_BLACKER_MAGICK(language, r_charray<3, 5>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        size_t bitcount = (Base::version == 1) ? 64 : 32;
        creation_time << read_args(r, bitcount);
        modification_time << read_args(r, bitcount);
        timescale << r;
        duration << read_args(r, bitcount);

        r.skip(1); // padding
        language << r;

        r.skip(16); // reserved
    }
};
} // namespace hrodvitnir::core::boxes
