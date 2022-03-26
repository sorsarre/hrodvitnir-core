// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/tree/tree-node.hpp>
#include <list>
#include <memory>

namespace hrodvitnir::core
{
class tree
{
  public:
    using ptr = std::shared_ptr<tree>;

    tree() = default;
    void add_child(tree_node::ptr child);
    const std::list<tree_node::ptr> children() const;

  private:
    tree(const tree&) = delete;
    tree& operator=(const tree&) = delete;

    std::list<tree_node::ptr> _children;
};
} // namespace hrodvitnir::core
