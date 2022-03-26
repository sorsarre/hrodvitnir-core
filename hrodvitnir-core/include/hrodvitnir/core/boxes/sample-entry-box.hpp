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
struct sample_entry : public Base
{
    MUCH_BLACKER_MAGICK(data_reference_index, r_uint<16>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        r.skip(8 * 6); // reserved
        data_reference_index << r;
    }
};
} // namespace hrodvitnir::core::boxes
