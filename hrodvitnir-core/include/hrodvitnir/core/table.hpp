// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
template <typename Entry>
class table
{
  public:
    using entry_type = Entry;

    virtual size_t size() const = 0;
    virtual const entry_type at(size_t index) const = 0;

    const entry_type& operator[](size_t index) const
    {
        return this->at(index);
    }

    virtual ~table() = default;
};

//--------------------------------------------------------------------------
template <typename Entry>
class array_table : public table<Entry>
{
  public:
    using entry_type = typename table<Entry>::entry_type;
    using container_type = std::vector<entry_type>;

    //----------------------------------------------------------------------
    array_table() = default;
    array_table(array_table<Entry>&& other) = default;

    //----------------------------------------------------------------------
    size_t size() const override
    {
        return _entries.size();
    }

    //----------------------------------------------------------------------
    const entry_type at(size_t index) const override
    {
        return _entries.at(index);
    }

    //----------------------------------------------------------------------
    const container_type& entries() const
    {
        return _entries;
    }

    //----------------------------------------------------------------------
    container_type& entries()
    {
        return _entries;
    }

  private:
    std::vector<Entry> _entries;
};

//--------------------------------------------------------------------------
template <typename Entry>
class fixed_table : public table<Entry>
{
  public:
    using entry_type = typename table<Entry>::entry_type;

    //----------------------------------------------------------------------
    fixed_table(fixed_table<entry_type>&& other) : _size(other._size), _entry(std::move(other._entry)) {}

    //----------------------------------------------------------------------
    fixed_table(entry_type entry, size_t size) : _size(size), _entry(std::move(entry)) {}

    //----------------------------------------------------------------------
    size_t size() const override
    {
        return _size;
    }

    //----------------------------------------------------------------------
    const entry_type at(size_t index) const override
    {
        if (index >= _size)
        {
            throw std::range_error("Index out of bounds in fixed table");
        }

        return _entry;
    }

    //----------------------------------------------------------------------
    const entry_type& entry() const
    {
        return _entry;
    }

    //----------------------------------------------------------------------
    entry_type& entry()
    {
        return _entry;
    }

  private:
    size_t _size;
    entry_type _entry;
};
} // namespace hrodvitnir::core