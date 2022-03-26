// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/file_reader.hpp>
#include <bitcommons/shared_buffer.hpp>

using namespace bitcommons;

namespace
{
//--------------------------------------------------------------------------
uint8_t* generate_test_data(size_t size)
{
    if (size == 0)
    {
        return nullptr;
    }

    auto ret = new uint8_t[size];
    for (size_t iter = 0; iter < size; ++iter)
    {
        ret[iter] = static_cast<uint8_t>(iter + 1);
    }
    return ret;
}

//--------------------------------------------------------------------------
class fake_file_reader : public file_reader
{
  public:
    //----------------------------------------------------------------------
    explicit fake_file_reader(size_t size)
    {
        _data = shared_buffer::wrap_mem(generate_test_data(size), size);
    }

    //----------------------------------------------------------------------
    size_t read(uint8_t* dest, uint64_t position, size_t bytes) override
    {
        if (position > _data.size())
        {
            throw std::runtime_error("Cannot seek beyond the end of the buffer");
        }

        size_t to_copy = std::min(_data.size() - position, bytes);
        auto begin = _data.begin() + position;
        auto end = begin + to_copy;
        std::copy(begin, end, dest);
        return to_copy;
    }

    //----------------------------------------------------------------------
    uint64_t size() override
    {
        return _data.size();
    }

    //----------------------------------------------------------------------
    std::shared_ptr<file_reader> clone() override
    {
        auto ret = new fake_file_reader(*this);
        return std::shared_ptr<file_reader>(ret);
    }

    //----------------------------------------------------------------------
    bool depleted() override
    {
        return true;
    }

    //----------------------------------------------------------------------
    ~fake_file_reader() override = default;

  private:
    fake_file_reader(const fake_file_reader& other)
    {
        _data = shared_buffer::clone(other._data);
    }

    shared_buffer _data;
};
} // namespace
