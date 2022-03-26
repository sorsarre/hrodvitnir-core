// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include "gtest/gtest.h"
#include <hrodvitnir/core/fourcc.hpp>

using namespace hrodvitnir::core;

//------------------------------------------------------------------------------
TEST(fourccTest, empty)
{
    fourcc fcc;
    EXPECT_EQ("\\x0\\\x0\\x0\\x0", fcc.string());
    EXPECT_EQ(0x00000000, fcc.value());
    EXPECT_EQ(fourcc{}, fcc);
}

//------------------------------------------------------------------------------
TEST(fourccTest, fromIntValue)
{
    static constexpr const uint32_t value = 0x61626463; // abdc
    fourcc fcc{value};
    EXPECT_EQ(value, fcc.value());
    EXPECT_EQ("abdc", fcc.string());
}

//------------------------------------------------------------------------------
TEST(fourccTest, fromString)
{
    static constexpr const char* string = "abdc";
    fourcc fcc{string};
    EXPECT_EQ(0x61626463, fcc.value());
    EXPECT_EQ("abdc", fcc.string());
}

//------------------------------------------------------------------------------
TEST(fourccTest, comparison)
{
    static constexpr const char* stringA = "abcd";
    static constexpr const char* stringB = "abdc";

    fourcc fccA{stringA};
    fourcc fccB{stringB};
    fourcc fccC{stringA};

    EXPECT_NE(fccA, fccB);
    EXPECT_LT(fccA, fccB);
    EXPECT_GT(fccB, fccA);
    EXPECT_EQ(fccA, fccC);
}
