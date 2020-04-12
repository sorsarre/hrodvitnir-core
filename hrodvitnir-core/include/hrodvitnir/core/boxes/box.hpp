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
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>
#include <hrodvitnir/core/uuid.hpp>

namespace hrodvitnir::core::boxes
{
    template<typename Base>
    struct box: public Base
    {
        MUCH_BLACKER_MAGICK(__pos, r_manual<uint64_t>);
        MUCH_BLACKER_MAGICK(__size, r_manual<uint64_t>);
        MUCH_BLACKER_MAGICK(size, r_uint<32>);
        MUCH_BLACKER_MAGICK(type, r_fourcc);
        MUCH_BLACKER_MAGICK(largesize, r_uint<64>);
        MUCH_BLACKER_MAGICK(uuid, r_uuid);

        [[nodiscard]]
        uint64_t box_size() const
        {
            return __size;
        }

        [[nodiscard]]
        uint64_t box_pos() const
        {
            return __pos;
        }

        [[nodiscard]]
        uint64_t box_end() const
        {
            return __pos() + __size();
        }

        template<typename Reader>
        void read_basic(Reader& r)
        {
            __pos = r.position() / 8; // TODO: Add byte-counting functions
            size << r;
            type << r;
            if (size == 1) {
                largesize << r;
                __size = largesize;
            } else if (size == 0) {
                largesize = r.available() / 8; // TODO: Add byte-counting functions
                __size = largesize;
            } else {
                __size = size;
            }

            if (type() == fourcc("uuid")) {
                uuid << r;
            } else {
                uuid = uuid::from_fourcc(type);
            }
        }

        template<typename Reader>
        void read(Reader& r)
        {
            // nope, at least until I find a better way to get around that
        }
    };
}
