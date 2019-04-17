/*
BSD 3-Clause License

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

#pragma once
#include <hrodvitnir/core/box-property.hpp>
#include <hrodvitnir/core/readspec.hpp>

namespace hrodvitnir::core::boxes
{

    template<typename Base>
    struct visual_sample_entry: public Base
    {
        MUCH_BLACKER_MAGICK(width, r_uint<16>);
        MUCH_BLACKER_MAGICK(height, r_uint<16>);
        MUCH_BLACKER_MAGICK(horizresolution, r_qmn<uint32_t, 16>);
        MUCH_BLACKER_MAGICK(vertresolution, r_qmn<uint32_t, 16>);
        MUCH_BLACKER_MAGICK(frame_count, r_uint<16>);
        MUCH_BLACKER_MAGICK(compressorname, r_array<32, r_char<8>>);
        MUCH_BLACKER_MAGICK(depth, r_uint<16>);

        template<typename Reader>
        void read(Reader& r)
        {
            Base::read(r);
            r.skip(16 + 16 + 32*3); // various reserved fields
            width << r;
            height << r;
            horizresolution << r;
            vertresolution << r;
            r.skip(32); // reserved
            frame_count << r;
            compressorname << r;
            depth << r;
            r.skip(16); // predefined
        }
    };
}
