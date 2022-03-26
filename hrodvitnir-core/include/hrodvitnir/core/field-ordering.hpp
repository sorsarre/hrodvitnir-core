// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <map>
#include <memory>

namespace hrodvitnir::core
{
class field_ordering_map
{
  public:
    template <typename BoxBinding>
    static std::shared_ptr<field_ordering_map> get_instance()
    {
        static std::shared_ptr<field_ordering_map> instance;
        if (!instance)
        {
            instance = std::make_shared<field_ordering_map>();
        }
        return instance;
    }

    using internal_type = std::map<size_t, uint64_t>;
    using const_iterator = internal_type::const_iterator;

    const_iterator begin() const
    {
        return _field_order.cbegin();
    }
    const_iterator cbegin() const
    {
        return _field_order.cbegin();
    }
    const_iterator end() const
    {
        return _field_order.cend();
    }
    const_iterator cend() const
    {
        return _field_order.cend();
    }

    void register_field(uint64_t key)
    {
        _field_order[_index] = key;
        ++_index;
    }

  private:
    std::map<uint64_t, size_t> _field_order;
    size_t _index = 0;
};
} // namespace hrodvitnir::core