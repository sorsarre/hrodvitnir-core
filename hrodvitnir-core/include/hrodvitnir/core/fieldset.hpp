// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <any>
#include <cstdint>
#include <hrodvitnir/core/ce_crc64.hpp>
#include <hrodvitnir/core/field-ordering.hpp>
#include <unordered_map>

namespace hrodvitnir::core
{
//------------------------------------------------------------------------------
// FIELDSET
//------------------------------------------------------------------------------
struct fieldset
{
    //----------------------------------------------------------------------
    const std::any& get(uint64_t key) const;
    std::any& get(uint64_t key);
    const std::any& get(const char* str) const;
    std::any& get(const char* str);
    const std::any& get(const std::string& str) const;
    std::any& get(const std::string& str);
    void set(uint64_t key, std::any&& val);
    void set(const char* str, std::any&& val);
    void set(const std::string& str, std::any&& val);
    bool has(uint64_t key) const
    {
        return _fields.count(key) > 0;
    }

    //----------------------------------------------------------------------
    std::unordered_map<uint64_t, std::any> _fields;
    std::shared_ptr<field_ordering_map> _ordering;
};
} // namespace hrodvitnir::core
