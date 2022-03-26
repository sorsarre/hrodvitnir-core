// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-consumer.hpp>
#include <hrodvitnir/core/tree/tree-node.hpp>
#include <hrodvitnir/core/tree/tree.hpp>
#include <stack>

namespace hrodvitnir::core
{
class tree_builder : public box_consumer
{
  public:
    using ptr = std::shared_ptr<tree_builder>;

    tree_builder();
    void box_announce(uint64_t position, const uuid& id) override;
    void box_open(const std::shared_ptr<fieldset>& fs) override;
    void box_close(uint64_t position) override;
    tree::ptr get() const;

  private:
    std::stack<tree_node::ptr> _stack;
    tree::ptr _tree;
};
} // namespace hrodvitnir::core
