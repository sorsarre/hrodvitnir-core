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
struct sample_dependency_type_entry
{
    uint8_t is_leading : 2;
    uint8_t sample_depends_on : 2;
    uint8_t sample_is_depended_on : 2;
    uint8_t sample_has_redundancy : 2;
};

template <typename Base>
struct sample_dependency_type : public Base
{
    using r_sdtp_entry = r_lambda < [](auto& r) -> auto
    {
        using r_uint2 = r_uint<2>;
        sample_dependency_type_entry ret;
        ret.is_leading = r_uint2::read(r);
        ret.sample_depends_on = r_uint2::read(r);
        ret.sample_is_depended_on = r_uint2::read(r);
        ret.sample_has_redundancy = r_uint2::read(r);
        return ret;
    }
    > ;

    using r_sdtp_table = r_filler_table<r_sdtp_entry>;

    MUCH_BLACKER_MAGICK(_contents, r_sdtp_table);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        auto pos = r.position_bytes();
        auto rest = Base::box_end() - pos;
        _contents << read_args(r, rest);
    }
};
} // namespace hrodvitnir::core::boxes
