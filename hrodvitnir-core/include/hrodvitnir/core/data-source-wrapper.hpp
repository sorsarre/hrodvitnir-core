// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <cstddef>
#include <cstdint>
#include <hrodvitnir/core/data-source.hpp>

namespace hrodvitnir::core
{
template <typename Original>
class data_source_wrapper : public data_source
{
  public:
    data_source_wrapper(const std::shared_ptr<Original>& orig) : _orig(orig) {}

    size_t get_n(uint64_t& buf, size_t bytes) override
    {
        return _orig->get_n(buf, bytes);
    }

    bool depleted() override
    {
        return _orig->depleted();
    }

    uint64_t available() override
    {
        return _orig->available();
    }

    uint64_t position() override
    {
        return _orig->position();
    }

    void seek(uint64_t position) override
    {
        _orig->seek(position);
    }

    void skip(uint64_t bytes) override
    {
        _orig->skip(bytes);
    }

    std::shared_ptr<data_source> clone() override
    {
        auto orig_clone = _orig->clone();
        return std::make_shared<data_source_wrapper<Original>>(orig_clone);
    }

  private:
    std::shared_ptr<Original> _orig;
};

template <typename Original>
std::shared_ptr<data_source_wrapper<Original>> wrap_data_source(const std::shared_ptr<Original>& orig)
{
    return std::make_shared<data_source_wrapper<Original>>(orig);
}
} // namespace hrodvitnir::core
