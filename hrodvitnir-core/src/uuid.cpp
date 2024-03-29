// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/uuid.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
bool uuid::is_iso() const
{
    return std::equal(data + 4, data + 16, ISO_12_reserved);
}

//--------------------------------------------------------------------------
uuid uuid::from_fourcc(const fourcc& fcc)
{
    return from_fourcc(fcc.value());
}

//--------------------------------------------------------------------------
uuid uuid::from_fourcc(uint32_t fourcc)
{
    uuid ret;
    std::copy(ISO_12_reserved, ISO_12_reserved + 12, ret.data + 4);
    ret.data[0] = static_cast<uint8_t>(fourcc >> 24);
    ret.data[1] = static_cast<uint8_t>((fourcc >> 16) & 0xFF);
    ret.data[2] = static_cast<uint8_t>((fourcc >> 8) & 0xFF);
    ret.data[3] = static_cast<uint8_t>(fourcc & 0xFF);

    return ret;
}

//--------------------------------------------------------------------------
std::string uuid::to_string() const
{
    std::string ret;
    convert_partial(ret, 0, 4);
    ret.push_back('-');
    convert_partial(ret, 4, 6);
    ret.push_back('-');
    convert_partial(ret, 6, 8);
    ret.push_back('-');
    convert_partial(ret, 8, 10);
    ret.push_back('-');
    convert_partial(ret, 10, 16);
    return ret;
}

//--------------------------------------------------------------------------
bool uuid::operator<(const uuid& other) const
{
    return std::lexicographical_compare(data, data + sizeof(data), other.data, other.data + sizeof(data));
}

//--------------------------------------------------------------------------
bool uuid::operator==(const uuid& other) const
{
    if (this == &other)
    {
        return true;
    }
    else
    {
        return std::equal(data, data + sizeof(data), other.data);
    }
}

//--------------------------------------------------------------------------
bool uuid::operator!=(const uuid& other) const
{
    return !(*this == other);
}

//--------------------------------------------------------------------------
void uuid::convert_partial(std::string& target, size_t begin, size_t end) const
{
    static const auto* conv = reinterpret_cast<const uint8_t*>("0123456789ABCDEF");
    for (size_t iter = begin; iter != end; ++iter)
    {
        target.push_back(conv[data[iter] & 0x0F]);
        target.push_back(conv[data[iter] >> 4]);
    }
}
} // namespace hrodvitnir::core
