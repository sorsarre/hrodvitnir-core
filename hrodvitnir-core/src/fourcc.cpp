// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/fourcc.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
std::string fourcc::string() const
{
    // TODO: Escape non-printale characters
    std::string ret = "0000";
    for (size_t iter = 0; iter < 4; ++iter)
    {
        ret[iter] = static_cast<char>((_data >> (8 * (3 - iter))) & 0xFF);
    }

    return ret;
}
} // namespace hrodvitnir::core
