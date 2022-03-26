// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/tree/tree-builder.hpp>

namespace hrodvitnir::core
{
//--------------------------------------------------------------------------
tree_builder::tree_builder()
{
    _tree = std::make_shared<tree>();
}

//--------------------------------------------------------------------------
void tree_builder::box_announce(uint64_t position, const uuid& id)
{
    // nope, nothing to do here
}

//--------------------------------------------------------------------------
void tree_builder::box_open(const std::shared_ptr<fieldset>& fs)
{
    auto node = std::make_shared<tree_node>(fs);
    if (_stack.empty())
    {
        _tree->add_child(node);
    }
    else
    {
        _stack.top()->add_child(node);
    }
    _stack.push(node);
}

//--------------------------------------------------------------------------
void tree_builder::box_close(uint64_t position)
{
    _stack.pop();
}

//--------------------------------------------------------------------------
tree::ptr tree_builder::get() const
{
    return _tree; // TODO: Make it an honest const
}
} // namespace hrodvitnir::core
