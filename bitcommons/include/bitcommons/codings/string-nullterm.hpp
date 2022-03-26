// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

#include <bitcommons/bitreader-utils.hpp>
#include <string>

namespace bitcommons::ext
{
struct string_nullterm : public bitcommons::binary_codec
{
    using value_type = std::string;

    template <typename Reader>
    static std::string read(Reader& br)
    {
        std::string ret;
        char current = br.template read<char>(8);
        while (current != '\0')
        {
            ret.push_back(current);
            current = br.template read<char>(8);
        }

        return ret;
    }

    template <typename Writer>
    static void write(Writer& w, const std::string& value)
    {
        for (char c : value)
        {
            w.write(c, 8);
        }
        w.write('\0', 8);
    }
};
} // namespace bitcommons::ext