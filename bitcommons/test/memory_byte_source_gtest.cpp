/* BSD 3-Clause License

Copyright (c) 2019-2020, Andrey Kaydalov
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

#include "gtest_common.hpp"

#include <bitcommons/memory_byte_source.hpp>
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
TEST(memoryByteSourceTest, emptyCtor)
{
    memory_byte_source src;
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
TEST(memoryByteSourceTest, basic)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

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

    delete[] data;
}

//------------------------------------------------------------------------------
TEST(memoryByteSourceTest, skip)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

    check_get(src, 1, 1);
    EXPECT_EQ(size - 1, src.available());
    EXPECT_NO_THROW(src.skip(size / 2 - 1));
    check_get(src, size / 2 + 1, 1);
    EXPECT_EQ(size / 2 - 1, src.available());

    EXPECT_ANY_THROW(src.skip(size / 2));
    check_get(src, size / 2 + 2, 1);
    EXPECT_EQ(size / 2 - 2, src.available());
}

//------------------------------------------------------------------------------
TEST(memoryByteSourceTest, seek)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

    check_get(src, 1, 1);
    EXPECT_EQ(size - 1, src.available());
    EXPECT_NO_THROW(src.seek(size / 2));
    check_get(src, size / 2 + 1, 1);
    EXPECT_EQ(size / 2 - 1, src.available());

    EXPECT_ANY_THROW(src.seek(size + 1));
    check_get(src, size / 2 + 2, 1);
    EXPECT_EQ(size / 2 - 2, src.available());
}

//------------------------------------------------------------------------------
TEST(memoryByteSourceTest, clone)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    memory_byte_source src(data, size);

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
