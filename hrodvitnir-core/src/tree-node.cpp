// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/tree/tree-node.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
tree_node::tree_node(std::shared_ptr<fieldset> fs) : _value(fs)
{
    _position = std::any_cast<uint64_t>(fs->get("__pos"_crc64));
    _size = std::any_cast<uint64_t>(fs->get("__size"_crc64));
}

//--------------------------------------------------------------------------
uint64_t tree_node::position() const
{
    return _position;
}

//--------------------------------------------------------------------------
uint64_t tree_node::size() const
{
    return _size;
}

//--------------------------------------------------------------------------
uint64_t tree_node::end() const
{
    return _position + _size;
}

//--------------------------------------------------------------------------
const std::vector<tree_node::ptr>& tree_node::children() const
{
    return _children;
}

//--------------------------------------------------------------------------
void tree_node::add_child(tree_node::ptr child)
{
    _children.push_back(child);
}

//--------------------------------------------------------------------------
const std::shared_ptr<fieldset>& tree_node::get() const
{
    return _value;
}
} // namespace hrodvitnir::core
