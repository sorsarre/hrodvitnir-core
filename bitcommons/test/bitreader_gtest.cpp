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
#include <bitcommons/memory_byte_source.hpp>
#include <bitcommons/bitreader.hpp>
#include <bitcommons/codings/exp-golomb-k0.hpp>

using namespace bitcommons;

using source_t = memory_byte_source;

//------------------------------------------------------------------------------
TEST(bitreaderTest, setData)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0, br.position());
    EXPECT_EQ(32, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_nonfull)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0xFF, br.read<uint8_t>(8));
    EXPECT_EQ(8, br.position());
    EXPECT_EQ(24, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_full)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0xFF112233, br.read<uint32_t>(32));
    EXPECT_EQ(32, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_nonaligned_nonfull)
{
    const uint8_t data[] = {0xFF, 0x11, 0x22, 0x33};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0xF, br.read<uint8_t>(4));
    EXPECT_EQ(4, br.position());
    EXPECT_EQ(28, br.available());
    EXPECT_EQ(0xF11223, br.read<uint32_t>(24));
    EXPECT_EQ(28, br.position());
    EXPECT_EQ(4, br.available());
    EXPECT_EQ(0x3, br.read<uint16_t>(4));
    EXPECT_EQ(32, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_64)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x1122334455667788, br.read<uint64_t>(64));
    EXPECT_EQ(64, br.position());
    EXPECT_EQ(8, br.available());
    EXPECT_EQ(0x99, br.read<uint8_t>(8));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_aligned_64_cross)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x11, br.read<uint8_t>(8));
    EXPECT_EQ(8, br.position());
    EXPECT_EQ(64, br.available());
    EXPECT_EQ(0x2233445566778899, br.read<uint64_t>(64));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_nonaligned_64_cross)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x01, br.read<uint8_t>(4));
    EXPECT_EQ(4, br.position());
    EXPECT_EQ(68, br.available());
    EXPECT_EQ(0x1223344556677889, br.read<uint64_t>(64));
    EXPECT_EQ(68, br.position());
    EXPECT_EQ(4, br.available());
    EXPECT_EQ(0x09, br.read<uint16_t>(4));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_signed_aligned)
{
    const uint8_t data[] = {0xFE, 0x3F};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(-2, br.read<int8_t>(8));
    EXPECT_EQ(0x3F, br.read<int8_t>(8));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, read_signed_nonaligned)
{
    const uint8_t data[] = {0xFE, 0x3F};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(10));
    EXPECT_EQ(-1, br.read<int8_t>(6));
    EXPECT_EQ(16, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_non_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(3*8));
    EXPECT_EQ(24, br.position());
    EXPECT_EQ(48, br.available());
    EXPECT_EQ(0x445, br.read<uint16_t>(12));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_non_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(7));
    EXPECT_EQ(7, br.position());
    EXPECT_EQ(33, br.available());
    EXPECT_EQ(0x122, br.peek<uint16_t>(9));
    EXPECT_EQ(7, br.position());
    EXPECT_EQ(33, br.available());
    EXPECT_NO_THROW(br.skip(21));
    EXPECT_EQ(28, br.position());
    EXPECT_EQ(12, br.available());
    EXPECT_EQ(0x455, br.read<uint16_t>(12));
    EXPECT_EQ(40, br.position());
    EXPECT_EQ(0, br.available());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(72));
    EXPECT_EQ(72, br.position());
    EXPECT_EQ(8, br.available());
    EXPECT_EQ(0xAA, br.read<uint8_t>(8));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, skip_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(74));
    EXPECT_EQ(74, br.position());
    EXPECT_EQ(6, br.available());
    EXPECT_EQ(0b10'1010, br.read<uint8_t>(6));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_non_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x1122, br.peek<uint16_t>(16));
    EXPECT_EQ(0, br.position());
    EXPECT_EQ(0x1122, br.read<uint16_t>(16));
    EXPECT_EQ(16, br.position());
    EXPECT_EQ(0x3344, br.peek<uint16_t>(16));
    EXPECT_EQ(16, br.position());
    EXPECT_EQ(0x3344, br.read<uint16_t>(16));
    EXPECT_EQ(32, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_non_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_EQ(0x112, br.peek<uint16_t>(12));
    EXPECT_EQ(0, br.position());
    EXPECT_EQ(0x112, br.read<uint16_t>(12));
    EXPECT_EQ(12, br.position());
    EXPECT_EQ(0x233, br.peek<uint16_t>(12));
    EXPECT_EQ(12, br.position());
    EXPECT_EQ(0x23344, br.read<uint32_t>(20));
    EXPECT_EQ(32, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_cross_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(48));
    EXPECT_EQ(48, br.position());
    EXPECT_EQ(0x778899, br.peek<uint32_t>(24));
    EXPECT_EQ(48, br.position());
    EXPECT_EQ(0x778899AA, br.read<uint32_t>(32));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, peek_cross_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(52));
    EXPECT_EQ(52, br.position());
    EXPECT_EQ(0x78899A, br.peek<uint32_t>(24));
    EXPECT_EQ(52, br.position());
    EXPECT_EQ(0x78899AA, br.read<uint32_t>(28));
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, align_lt_byte_aligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.align(5));
    EXPECT_EQ(0, br.position());
    EXPECT_NO_THROW(br.skip(10));
    EXPECT_EQ(10, br.position());
    EXPECT_NO_THROW(br.align(5));
    EXPECT_EQ(10, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, align_lt_byte_unaligned)
{
    const uint8_t data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xAA};
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);
    EXPECT_NO_THROW(br.skip(2));
    EXPECT_EQ(2, br.position());
    EXPECT_NO_THROW(br.align(5));
    EXPECT_EQ(5, br.position());
}

//------------------------------------------------------------------------------
TEST(bitreaderTest, exp_golomb_k0)
{
    const uint8_t data[] = {0b1'010'011'0, 0b0100'0000 };
    auto source = std::make_shared<source_t>(data, sizeof(data));
    bitreader<source_t> br(source);

    using egc = bitcommons::ext::exp_golomb_k0<uint8_t>;

    EXPECT_EQ(0, br.read<egc>());
    EXPECT_EQ(1, br.read<egc>());
    EXPECT_EQ(2, br.read<egc>());
    EXPECT_EQ(3, br.read<egc>());
}
