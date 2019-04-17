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

#include <gtest/gtest.h>
#include <bitcommons/shared_buffer.hpp>

using namespace bitcommons;

namespace {
    uint8_t* create_test_data(size_t count)
    {
        auto ret = new uint8_t[count];
        for (size_t iter = 0; iter < count; ++iter) {
            ret[iter] = static_cast<uint8_t>(iter);
        }

        return ret;
    }
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, testCtor)
{
    shared_buffer buf;
    EXPECT_EQ(nullptr, buf.get());
    EXPECT_EQ(0, buf.capacity());
    EXPECT_EQ(0, buf.size());
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, wrap_mem)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    shared_buffer buf = shared_buffer::wrap_mem(data, size);
    EXPECT_EQ(data, buf.get());
    EXPECT_EQ(size, buf.size());
    EXPECT_EQ(size, buf.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[size-1]);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, copy_mem)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::copy_mem(data, size);
    EXPECT_NE(data, buf.get());
    EXPECT_EQ(size, buf.size());
    EXPECT_EQ(size, buf.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[size-1]);
    EXPECT_TRUE(std::equal(data, data+size, buf.get()));
    delete[] data;
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, clone)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf1 = shared_buffer::wrap_mem(data, size);
    auto buf2 = shared_buffer::clone(buf1);
    EXPECT_NE(buf1.get(), buf2.get());
    EXPECT_EQ(buf1.size(), buf2.size());
    EXPECT_EQ(buf1.capacity(), buf2.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[size-1]);
    EXPECT_TRUE(std::equal(buf2.cbegin(), buf2.cend(), buf1.cbegin()));
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, checkNull)
{
    shared_buffer buf;
    EXPECT_FALSE(buf);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, checkNonNull)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::wrap_mem(data, size);
    EXPECT_TRUE(buf);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, copy)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf1 = shared_buffer::wrap_mem(data, size);
    shared_buffer buf2;
    buf2 = buf1;
    EXPECT_EQ(buf1.get(), buf2.get());
    EXPECT_EQ(buf1.size(), buf2.size());
    EXPECT_EQ(buf1.capacity(), buf2.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[size-1]);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, copyCtor)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf1 = shared_buffer::wrap_mem(data, size);
    auto buf2 = buf1;
    EXPECT_EQ(buf1.get(), buf2.get());
    EXPECT_EQ(buf1.size(), buf2.size());
    EXPECT_EQ(buf1.capacity(), buf2.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[size-1]);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, moveCtor)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf1 = shared_buffer::wrap_mem(data, size);
    auto buf2 = std::move(buf1);
    EXPECT_EQ(data, buf2.get());
    EXPECT_EQ(size, buf2.size());
    EXPECT_EQ(size, buf2.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf2.get()[size-1]);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, resizeZero)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf1 = shared_buffer::wrap_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf1.resize(0));
    EXPECT_TRUE(buf1);
    EXPECT_EQ(data, buf1.get());
    EXPECT_EQ(0, buf1.size());
    EXPECT_EQ(size, buf1.capacity());
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, resizeLess)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf1 = shared_buffer::wrap_mem(data, size);
    const size_t new_size = size/2;
    EXPECT_NO_FATAL_FAILURE(buf1.resize(new_size));
    EXPECT_TRUE(buf1);
    EXPECT_EQ(data, buf1.get());
    EXPECT_EQ(new_size, buf1.size());
    EXPECT_EQ(size, buf1.capacity());
    EXPECT_TRUE(std::equal(data, data+new_size, buf1.cbegin()));
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, resizeEqual)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::wrap_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf.resize(size));
    EXPECT_TRUE(buf);
    EXPECT_EQ(data, buf.get());
    EXPECT_EQ(size, buf.size());
    EXPECT_EQ(size, buf.size());
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, resizeLarger)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::copy_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf.resize(size*2));
    EXPECT_TRUE(buf);
    EXPECT_NE(data, buf.get());
    EXPECT_EQ(size*2, buf.size());
    EXPECT_EQ(size*2, buf.capacity());
    EXPECT_TRUE(std::equal(data, data+size, buf.cbegin()));
    delete[] data;
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, reallocZero)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::wrap_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf.realloc(0));
    EXPECT_FALSE(buf);
    EXPECT_EQ(nullptr, buf.get());
    EXPECT_EQ(0, buf.size());
    EXPECT_EQ(0, buf.capacity());
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, reallocLess)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::copy_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf.realloc(size/2));
    EXPECT_TRUE(buf);
    EXPECT_NE(data, buf.get());
    EXPECT_EQ(size/2, buf.size());
    EXPECT_EQ(size/2, buf.capacity());
    EXPECT_TRUE(std::equal(buf.cbegin(), buf.cend(), data));
    delete[] data;
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, reallocEqual)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::wrap_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf.realloc(size));
    EXPECT_TRUE(buf);
    EXPECT_EQ(data, buf.get());
    EXPECT_EQ(size, buf.size());
    EXPECT_EQ(size, buf.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[size-1]);
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, reallocLarger)
{
    const size_t size = 4;
    auto data = create_test_data(size);
    auto buf = shared_buffer::copy_mem(data, size);
    EXPECT_NO_FATAL_FAILURE(buf.realloc(size*2));
    EXPECT_TRUE(buf);
    EXPECT_EQ(size, buf.size());
    EXPECT_EQ(size*2, buf.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[size*2-1]);
    EXPECT_TRUE(std::equal(data, data+size, buf.cbegin()));
    delete[] data;
}

//------------------------------------------------------------------------------
TEST(sharedBufferTest, allocate)
{
    const size_t size = 4;
    auto buf = shared_buffer::allocate(size);
    EXPECT_TRUE(buf);
    EXPECT_EQ(0, buf.size());
    EXPECT_EQ(size, buf.capacity());
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[0]);
    EXPECT_NO_FATAL_FAILURE((void)buf.get()[size-1]);
}
