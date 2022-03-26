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
struct sound_media_header : public Base
{
    MUCH_BLACKER_MAGICK(balance, r_qmn<uint16_t, 8>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        balance << r;
        r.skip(16); // reserved
    }
};
} // namespace hrodvitnir::core::boxes
