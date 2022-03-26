// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <cstdint>
#include <hrodvitnir/core/fieldset.hpp>
#include <memory>
#include <unordered_map>
#include <vector>

namespace hrodvitnir::core
{
class tree_node
{
  public:
    using ptr = std::shared_ptr<tree_node>;

    tree_node(std::shared_ptr<fieldset> fs);
    uint64_t position() const;
    uint64_t size() const;
    uint64_t end() const;
    const std::vector<tree_node::ptr>& children() const;
    void add_child(tree_node::ptr child);
    const std::shared_ptr<fieldset>& get() const;

    tree_node() = delete;
    tree_node(const tree_node&) = delete;
    tree_node& operator=(const tree_node&) = delete;

  private:
    uint64_t _position;
    uint64_t _size;
    std::shared_ptr<fieldset> _value;
    std::vector<tree_node::ptr> _children;
};
} // namespace hrodvitnir::core
