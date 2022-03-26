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
struct handler : public Base
{
    MUCH_BLACKER_MAGICK(handler_type, r_fourcc);
    MUCH_BLACKER_MAGICK(name, r_string);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        r.skip(32); // pre_defined = 0;
        handler_type << r;
        r.skip(32 * 3); // reserved
        name << r;
    }
};
} // namespace hrodvitnir::core::boxes
