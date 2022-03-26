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
struct colour_information : public Base
{
    MUCH_BLACKER_MAGICK(colour_type, r_fourcc);

    // nclx
    MUCH_BLACKER_MAGICK(colour_primaries, r_uint<16>);
    MUCH_BLACKER_MAGICK(transfer_characteristics, r_uint<16>);
    MUCH_BLACKER_MAGICK(matrix_coefficients, r_uint<16>);
    MUCH_BLACKER_MAGICK(full_range_flag, r_uint<7>);
    MUCH_BLACKER_MAGICK(reserved, r_uint<7>);

    // nclc
    MUCH_BLACKER_MAGICK(primaries_index, r_uint<16>);
    MUCH_BLACKER_MAGICK(transfer_function_index, r_uint<16>);
    MUCH_BLACKER_MAGICK(matrix_index, r_uint<16>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        colour_type << r;
        if (colour_type() == "nclx")
        {
            colour_primaries << r;
            transfer_characteristics << r;
            matrix_coefficients << r;
            full_range_flag << r;
            reserved << r;
        }
        else if (colour_type() == "nclc")
        {
            primaries_index << r;
            transfer_function_index << r;
            matrix_index << r;
        }
    }
};
} // namespace hrodvitnir::core::boxes
