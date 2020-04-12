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

#include <csignal>
#include "gtest/gtest.h"
#include "gtest_common.hpp"

//------------------------------------------------------------------------------
TEST(testData, zeroSize)
{
    auto data = generate_test_data(0);
    EXPECT_EQ(nullptr, data);
}

//------------------------------------------------------------------------------
TEST(testData, contents)
{
    const size_t size = 10;
    auto data = generate_test_data(size);
    EXPECT_NE(nullptr, data);
    for (size_t iter = 0; iter < size; ++iter) {
        EXPECT_EQ(iter+1, data[iter]);
    }
    delete[] data;
}

//------------------------------------------------------------------------------
TEST(fakeFileReaderTest, zeroSize)
{
    fake_file_reader reader(0);
    EXPECT_EQ(0, reader.size());
    EXPECT_TRUE(reader.depleted());

    size_t read = 1;
    EXPECT_NO_THROW(read = reader.read(nullptr, 0, 0));
    EXPECT_EQ(0, read);
    EXPECT_NO_THROW(read = reader.read(nullptr, 0, 1));
    EXPECT_EQ(0, read);
    EXPECT_ANY_THROW(reader.read(nullptr, 1, 1));
    EXPECT_ANY_THROW(reader.read(nullptr, 1, 0));
}

//------------------------------------------------------------------------------
TEST(fakeFileReaderTest, nonZeroSize)
{
    const size_t size = 10;
    fake_file_reader reader(size);
    EXPECT_EQ(size, reader.size());
    EXPECT_TRUE(reader.depleted());

    size_t read = 1;
    uint8_t buffer[size];
    EXPECT_NO_THROW(read = reader.read(buffer, 0, 0));
    EXPECT_EQ(0, read);

    EXPECT_NO_THROW(read = reader.read(buffer, 0, size/2));
    EXPECT_EQ(size/2, read);
    for (size_t iter = 0; iter < size/2; ++iter) {
        EXPECT_EQ(iter+1, buffer[iter]);
    }

    EXPECT_NO_THROW(read = reader.read(buffer, size/2, size/2));
    EXPECT_EQ(size/2, read);
    for (size_t iter = size/2; iter < size/2; ++iter) {
        EXPECT_EQ(iter+1, buffer[iter]);
    }

    EXPECT_NO_THROW(read = reader.read(buffer, size, size/2));
    EXPECT_EQ(0, read);

    EXPECT_ANY_THROW(reader.read(buffer, size+1, size/2));
    EXPECT_ANY_THROW(reader.read(buffer, size+1, 0));
}
