// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-property.hpp>

namespace hrodvitnir::core::boxes
{
template <typename Base>
struct skipper : public Base
{
    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        r.seek_bytes(Base::box_end());
    }
};
} // namespace hrodvitnir::core::boxes
