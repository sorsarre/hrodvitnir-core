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
#include <cstdint>
#include <cstddef>
#include <string>

namespace hrodvitnir::core
{
    class fourcc
    {
    public:
        //----------------------------------------------------------------------
        constexpr fourcc(): _data(0) {};
        constexpr fourcc(uint32_t value): _data(value) {}
        constexpr fourcc(const char* str, size_t sz = 4): _data(from_string(str, sz)) {}

        //----------------------------------------------------------------------
        constexpr bool operator!=(const fourcc& other) const
        {
            return other._data != _data;
        }

        //----------------------------------------------------------------------
        constexpr bool operator==(const fourcc& other) const
        {
            return other._data == _data;
        }

        //----------------------------------------------------------------------
        constexpr bool operator<(const fourcc& other) const
        {
            return _data < other._data;
        }

        //----------------------------------------------------------------------
        constexpr bool operator>(const fourcc& other) const
        {
            return _data > other._data;
        }

        //----------------------------------------------------------------------
        std::string string() const;

        //----------------------------------------------------------------------
        constexpr uint32_t value() const { return _data; }

    private:
        //----------------------------------------------------------------------
        static constexpr uint32_t from_string(const char* s, size_t sz)
        {
            uint32_t ret = 0;
            for (size_t iter = 0; iter < sz; ++iter) {
                ret <<= 8;
                ret |= s[iter];
            }
            return ret;
        }

        uint32_t _data;
    };
}
