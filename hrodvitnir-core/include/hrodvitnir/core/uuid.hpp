/*
BSD 3-Clause License

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

#pragma once
#include <algorithm>
#include <cstdint>
#include <hrodvitnir/core/fourcc.hpp>

namespace hrodvitnir::core
{
static constexpr const uint8_t ISO_12_reserved[] = {0x00, 0x11, 0x00, 0x10, 0x80, 0x00,
                                                    0x00, 0xAA, 0x00, 0x38, 0x9B, 0x71};

class uuid
{
  public:
    //----------------------------------------------------------------------
    bool is_iso() const;
    static uuid from_fourcc(const fourcc& fcc);
    static uuid from_fourcc(uint32_t fourcc);
    std::string to_string() const;
    bool operator<(const uuid& other) const;
    bool operator==(const uuid& other) const;
    bool operator!=(const uuid& other) const;

    const uint8_t* begin() const
    {
        return data;
    }
    const uint8_t* end() const
    {
        return data + 16;
    }
    uint8_t* begin()
    {
        return data;
    }
    uint8_t* end()
    {
        return data;
    }

    template <typename ForwardIterator, typename Iterator>
    static uuid from_bytes(ForwardIterator begin, Iterator end)
    {
        uuid ret;
        std::copy(begin, end, ret.data);
        return ret;
    }

  private:
    void convert_partial(std::string& target, size_t begin, size_t end) const;

    uint8_t data[16] = {0};
};
} // namespace hrodvitnir::core
