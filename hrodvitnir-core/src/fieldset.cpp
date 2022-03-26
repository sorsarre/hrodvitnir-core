// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/fieldset.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
const std::any& fieldset::get(uint64_t key) const
{
    return _fields.at(key);
}

//--------------------------------------------------------------------------
std::any& fieldset::get(uint64_t key)
{
    return _fields.at(key);
}

//--------------------------------------------------------------------------
const std::any& fieldset::get(const char* str) const
{
    return _fields.at(const_crc64(str));
}

//--------------------------------------------------------------------------
std::any& fieldset::get(const char* str)
{
    return _fields.at(const_crc64(str));
}

//--------------------------------------------------------------------------
const std::any& fieldset::get(const std::string& str) const
{
    return _fields.at(const_crc64(str.c_str()));
}

//--------------------------------------------------------------------------
std::any& fieldset::get(const std::string& str)
{
    return _fields.at(const_crc64(str.c_str()));
}

//--------------------------------------------------------------------------
void fieldset::set(uint64_t key, std::any&& val)
{
    _fields[key] = val;
}

//--------------------------------------------------------------------------
void fieldset::set(const char* str, std::any&& val)
{
    set(const_crc64(str), std::move(val));
}

//--------------------------------------------------------------------------
void fieldset::set(const std::string& str, std::any&& val)
{
    set(str.c_str(), std::move(val));
}
} // namespace hrodvitnir::core
