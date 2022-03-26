// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <cstddef>

namespace hrodvitnir::meta
{

template <typename T>
struct id_t
{
    using type = T;
};

template <bool Cond, typename Then, typename Else>
struct if_t
{
};

template <typename Then, typename Else>
struct if_t<true, Then, Else>
{
    using type = Then;
};

template <typename Then, typename Else>
struct if_t<false, Then, Else>
{
    using type = Else;
};

template <size_t Bits, typename First, typename... Types>
struct pick_type
{
    using type = typename if_t<Bits <= sizeof(First) * 8, First, typename pick_type<Bits, Types...>::type>::type;
};

template <size_t Bits, typename First>
struct pick_type<Bits, First>
{
    static_assert(Bits <= sizeof(First) * 8, "No suitable type found for reading so many bits!");
    using type = First;
};

} // namespace hrodvitnir::meta
