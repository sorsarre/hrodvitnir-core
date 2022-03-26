// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/tree/tree.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
void tree::add_child(tree_node::ptr child)
{
    _children.push_back(child);
}

//--------------------------------------------------------------------------
const std::list<tree_node::ptr> tree::children() const
{
    return _children;
}
} // namespace hrodvitnir::core
