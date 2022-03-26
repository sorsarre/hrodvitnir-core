// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>
#include <hrodvitnir/core/table.hpp>

namespace hrodvitnir::core::boxes
{
template <typename Base>
struct sample_size : public Base
{
    using r_stsz_entry = r_uint<32>;
    using r_stsz_table = r_array_table<r_stsz_entry, r_uint<32>>;

    MUCH_BLACKER_MAGICK(_contents, r_stsz_table);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        auto default_sample_size = r_uint<32>::read(r);
        if (default_sample_size == 0)
        {
            _contents << r;
        }
        else
        {
            using table_t = fixed_table<uint32_t>;

            auto sample_count = r_uint<32>::read(r);
            auto table = std::make_shared<table_t>(default_sample_size, sample_count);
            _contents = table;
        }
    }
};
} // namespace hrodvitnir::core::boxes
