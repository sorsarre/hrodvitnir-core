// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-defs.hpp>
#include <hrodvitnir/core/parser/reader-mapping.hpp>
#include <map>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
class simple_reader_mapping : public reader_mapping
{
  public:
    simple_reader_mapping(const partial_reader_type& def_rd = &default_reader);
    const partial_reader_type& get(const uuid& u) const override;

    template <typename BoxBinding>
    simple_reader_mapping& assign(const std::initializer_list<fourcc>& fcc)
    {
        for (const auto& f : fcc)
        {
            _registry.emplace(uuid::from_fourcc(f), &partial_read<BoxBinding>);
        }
        return *this;
    }

  private:
    static void default_reader(fieldset& fs, reader_type& r);

    template <typename BoxBinding>
    static void partial_read(fieldset& fs, reader_type& reader)
    {
        BoxBinding box{fs};
        box.read(reader);
    }

    //----------------------------------------------------------------------
    std::map<uuid, partial_reader_type> _registry;
    partial_reader_type _default;
};
} // namespace hrodvitnir::core
