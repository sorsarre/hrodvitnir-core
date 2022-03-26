// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/bitreader-utils.hpp>
#include <cstddef>
#include <utility>

namespace hrodvitnir::brext
{
template <typename T>
struct array_reader
{
    template <typename Reader, typename Iter, typename... Args>
    static void read(Reader& r, Iter iter, size_t count, Args&&... args)
    {
        for (size_t i = 0; i < count; ++i)
        {
            *iter = r.template read<T>(std::forward<Args>(args)...);
        }
    }
};
} // namespace hrodvitnir::brext
