// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <bitcommons/file_reader.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

namespace bitcommons
{
class direct_file_reader : public file_reader
{
  public:
    size_t read(uint8_t* dest, uint64_t position, size_t bytes) override;
    uint64_t size() override;
    bool depleted() override;
    ~direct_file_reader() override;
    std::shared_ptr<file_reader> clone() override;
    static std::shared_ptr<file_reader> open(const std::string& path);

  private:
    explicit direct_file_reader(const std::string& path);

    const std::string _path;
    FILE* _file;
};
} // namespace bitcommons
