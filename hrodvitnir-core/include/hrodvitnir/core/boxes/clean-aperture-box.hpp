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
struct clean_aperture : public Base
{
    // TODO: replace with a rational
    MUCH_BLACKER_MAGICK(cleanApertureWidthN, r_uint<32>);
    MUCH_BLACKER_MAGICK(cleanApertureWidthD, r_uint<32>);

    MUCH_BLACKER_MAGICK(cleanApertureHeightN, r_uint<32>);
    MUCH_BLACKER_MAGICK(cleanApertureHeightD, r_uint<32>);

    MUCH_BLACKER_MAGICK(horizOffN, r_uint<32>);
    MUCH_BLACKER_MAGICK(horizOffD, r_uint<32>);

    MUCH_BLACKER_MAGICK(vertOffN, r_uint<32>);
    MUCH_BLACKER_MAGICK(vertOffD, r_uint<32>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        cleanApertureWidthN << r;
        cleanApertureWidthD << r;

        cleanApertureHeightN << r;
        cleanApertureHeightD << r;

        horizOffN << r;
        horizOffD << r;

        vertOffN << r;
        vertOffD << r;
    }
};
} // namespace hrodvitnir::core::boxes
