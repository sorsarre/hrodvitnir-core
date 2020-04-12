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

#include <gtest/gtest.h>
#include <bitcommons/bitwriter.hpp>
#include <bitcommons/codings/string-nullterm.hpp>
#include <bitcommons/codings/exp-golomb-k0.hpp>
#include <vector>

using namespace bitcommons;

//------------------------------------------------------------------------------
namespace {
    class TestWriterSink
    {
    public:

        void put(uint8_t data, size_t bits)
        {
            _data.push_back(data);
        }

        size_t position() const
        {
            return _data.size();
        }

        const std::vector<uint8_t>& data() const
        {
            return _data;
        }

    private:
        std::vector<uint8_t> _data;
    };

    using bytes = std::vector<uint8_t>;
}


//------------------------------------------------------------------------------
TEST(bitwriterTest, Create)
{
    auto sink = std::make_shared<TestWriterSink>();
    EXPECT_NO_FATAL_FAILURE(bitwriter{sink});
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, InitialState)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    EXPECT_EQ(0, w.position());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteLessThanBufferNoOffset)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    ASSERT_NO_THROW(w.write(0xFD, 3));
    EXPECT_EQ(0, sink->position());
    EXPECT_EQ(3, w.position());

    ASSERT_NO_THROW(w.flush());
    ASSERT_EQ(1, sink->position());
    EXPECT_EQ(bytes({0xA0}), sink->data());
    EXPECT_EQ(8, w.position());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteLessThanBufferWithOffset)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);
    ASSERT_NO_THROW(w.write(0xFF, 3));
    EXPECT_EQ(3, w.position());

    ASSERT_NO_THROW(w.write(0b1010, 4));
    EXPECT_EQ(7, w.position());
    w.flush();
    ASSERT_EQ(1, sink->position());
    EXPECT_EQ(bytes({0xF4}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteExactBufferNoOffset)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    ASSERT_NO_THROW(w.write(0x57, 8));
    EXPECT_EQ(8, w.position());
    ASSERT_EQ(1, sink->position());
    EXPECT_EQ(bytes({0x57}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteExactBufferWithOffset)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    ASSERT_NO_THROW(w.write(0xFF, 3));
    EXPECT_EQ(3, w.position());

    ASSERT_NO_THROW(w.write(0b11010, 5));
    EXPECT_EQ(8, w.position());
    ASSERT_EQ(1, sink->position());
    EXPECT_EQ(bytes({0xFA}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteMoreThanBufferNoOffset)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    ASSERT_NO_THROW(w.write(0x112, 12));
    EXPECT_EQ(12, w.position());

    w.flush();
    ASSERT_EQ(2, sink->position());
    EXPECT_EQ(bytes({0x11, 0x20}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteMoreThanBufferWithOffset)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    w.write(0b0110, 4);
    EXPECT_EQ(4, w.position());

    w.write(0x11, 8);
    EXPECT_EQ(12, w.position());
    w.flush();
    ASSERT_EQ(2, sink->position());
    EXPECT_EQ(bytes({0x61, 0x10}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteMoreThanBufferToEdge)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    w.write(0b0110, 4);
    EXPECT_EQ(4, w.position());

    w.write(0x112, 12);
    EXPECT_EQ(16, w.position());
    ASSERT_EQ(2, sink->position());
    EXPECT_EQ(bytes({0x61, 0x12}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, WriteMoreThanTwoBuffers)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    w.write(6, 4);
    EXPECT_EQ(4, w.position());

    w.write(0x112234, 24);
    EXPECT_EQ(28, w.position());
    ASSERT_EQ(3, sink->position());
    w.flush();
    ASSERT_EQ(4, sink->position());
    EXPECT_EQ(bytes({0x61, 0x12, 0x23, 0x40}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, TestAlign)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    w.write(6, 4);
    EXPECT_EQ(4, w.position());
    ASSERT_NO_THROW(w.align(7));
    EXPECT_EQ(7, w.position());
    w.write(1, 1);
    EXPECT_EQ(8, w.position());
    EXPECT_EQ(bytes({0x61}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, TestSkip)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w(sink);

    w.write(6, 4);
    w.skip(13);
    w.write(0b111, 3);
    EXPECT_EQ(20, w.position());
    w.flush();
    EXPECT_EQ(bytes({0x60, 0x00, 0x70}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, TestStringWriter)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w{sink};

    w.write<ext::string_nullterm>("test");
    EXPECT_EQ(bytes({'t', 'e', 's', 't', '\0'}), sink->data());
}

//------------------------------------------------------------------------------
TEST(bitwriterTest, TestExpGolombK0)
{
    auto sink = std::make_shared<TestWriterSink>();
    bitwriter w{sink};

    w.write<ext::exp_golomb_k0<uint8_t>>(0b1101);
    w.write<ext::exp_golomb_k0<uint8_t>>(0b0);
    w.flush();
    EXPECT_EQ(bytes({0b00011011}), sink->data());
}
