// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include "gtest_common.hpp"

#include <bitcommons/file_byte_source.hpp>
#include <gtest/gtest.h>

using namespace bitcommons;

//------------------------------------------------------------------------------
template <typename Source>
void check_get(Source& src, uint64_t val, size_t read)
{
    uint64_t buf = 0;
    EXPECT_EQ(read, src.get_n(buf, read));
    EXPECT_EQ(buf, val);
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, emptyCtor)
{
    const size_t size = 0;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    uint64_t buf = 0;
    EXPECT_ANY_THROW(src.get_n(buf, 1));
    EXPECT_TRUE(src.depleted());
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.seek(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.seek(1));
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.skip(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.skip(1));
    EXPECT_EQ(0, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, basic)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    for (size_t iter = 0; iter < size; ++iter)
    {
        EXPECT_EQ(size - iter, src.available());
        EXPECT_EQ(true, src.depleted());
        check_get(src, iter + 1, 1);
    }

    uint64_t buf = 0;
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.get_n(buf, 1));
    EXPECT_NO_THROW(src.seek(size));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.seek(size + 1));
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.skip(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.skip(1));
    EXPECT_EQ(0, src.available());

    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.get_n(buf, 1));
    EXPECT_NO_THROW(src.seek(size));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.seek(size + 1));
    EXPECT_EQ(0, src.available());
    EXPECT_NO_THROW(src.skip(0));
    EXPECT_EQ(0, src.available());
    EXPECT_ANY_THROW(src.skip(1));
    EXPECT_EQ(0, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, skip)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    check_get(src, 1, 1);
    EXPECT_EQ(size - 1, src.available());
    EXPECT_NO_THROW(src.skip(size / 2 - 1));
    EXPECT_EQ(src.position(), size / 2);
    check_get(src, size / 2 + 1, 1);
    EXPECT_EQ(size / 2 - 1, src.available());

    EXPECT_ANY_THROW(src.skip(size / 2));
    check_get(src, size / 2 + 2, 1);
    EXPECT_EQ(size / 2 - 2, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, seek)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    check_get(src, 1, 1);
    EXPECT_EQ(size - 1, src.available());
    EXPECT_NO_THROW(src.seek(size / 2));
    EXPECT_EQ(src.position(), size / 2);
    check_get(src, size / 2 + 1, 1);
    EXPECT_EQ(size / 2 - 1, src.available());

    EXPECT_ANY_THROW(src.seek(size + 1));
    check_get(src, size / 2 + 2, 1);
    EXPECT_EQ(size / 2 - 2, src.available());
}

//------------------------------------------------------------------------------
TEST(fileByteSourceTest, clone)
{
    const size_t size = 10;
    auto data = std::make_shared<fake_file_reader>(size);
    file_byte_source src(data);

    EXPECT_NO_THROW(src.seek(size / 2));
    auto clone = src.clone();

    EXPECT_EQ(src.position(), clone->position());
    EXPECT_EQ(src.available(), clone->available());
    uint64_t buf1 = 0;
    uint64_t buf2 = 0;
    EXPECT_EQ(1, src.get_n(buf1, 1));
    EXPECT_EQ(1, clone->get_n(buf2, 1));
    EXPECT_EQ(buf1, buf2);
}
