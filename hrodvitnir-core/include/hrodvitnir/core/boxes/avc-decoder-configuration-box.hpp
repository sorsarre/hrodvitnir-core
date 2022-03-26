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
struct avc_decoder_configuration : public Base
{
    MUCH_BLACKER_MAGICK(configurationVersion, r_uint<8>);
    MUCH_BLACKER_MAGICK(AVCProfileIndication, r_uint<8>);
    MUCH_BLACKER_MAGICK(profile_compatibility, r_uint<8>);
    MUCH_BLACKER_MAGICK(AVCLevelIndication, r_uint<8>);
    MUCH_BLACKER_MAGICK(lengthSizeMinusOne, r_uint<2>);

    using r_sps = r_items_sized<std::vector, r_uint<8>, r_uint<16>>;
    using r_pps = r_sps;

    using r_sps_set = r_items_sized<std::vector, r_sps, r_uint<5>>;
    using r_pps_set = r_items_sized<std::vector, r_pps, r_uint<8>>;

    MUCH_BLACKER_MAGICK(sequenceParameterSets, r_sps_set);
    MUCH_BLACKER_MAGICK(pictureParameterSets, r_pps_set);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        configurationVersion << r;
        AVCLevelIndication << r;
        profile_compatibility << r;
        AVCLevelIndication << r;
        r.skip(6);
        lengthSizeMinusOne << r;
        r.skip(3);
        sequenceParameterSets << r;
        pictureParameterSets << r;
    }
};
} // namespace hrodvitnir::core::boxes
