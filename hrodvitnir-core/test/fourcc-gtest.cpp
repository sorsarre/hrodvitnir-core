/* BSD 3-Clause License

Copyright (c) 2019, Andrey Kaydalov
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
