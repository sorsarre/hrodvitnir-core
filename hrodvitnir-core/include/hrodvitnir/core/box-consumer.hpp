// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/fieldset.hpp>
#include <hrodvitnir/core/uuid.hpp>

namespace hrodvitnir::core
{
class box_consumer
{
  public:
    using ptr = std::shared_ptr<box_consumer>;

    virtual void box_announce(uint64_t position, const uuid& id) = 0;
    virtual void box_open(const std::shared_ptr<fieldset>& fs) = 0;
    virtual void box_close(uint64_t position) = 0;

    virtual ~box_consumer() = default;
};
} // namespace hrodvitnir::core
