// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/parser/simple-reader-mapping.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
simple_reader_mapping::simple_reader_mapping(const partial_reader_type& def_rd)
{
    _default = def_rd;
}

//--------------------------------------------------------------------------
const partial_reader_type& simple_reader_mapping::get(const uuid& u) const
{
    auto iter = _registry.find(u);
    if (iter != _registry.end())
    {
        return iter->second;
    }
    else
    {
        return _default;
    }
}

//--------------------------------------------------------------------------
void simple_reader_mapping::default_reader(fieldset& fs, reader_type& r)
{
    boxes::default_box_t{fs}.read(r);
}
} // namespace hrodvitnir::core
