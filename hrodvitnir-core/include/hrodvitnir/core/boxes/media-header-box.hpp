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
    struct media_header: public Base
    {
        MUCH_BLACKER_MAGICK(creation_time, r_arg_bits<uint64_t>);
        MUCH_BLACKER_MAGICK(modification_time, r_arg_bits<uint64_t>);
        MUCH_BLACKER_MAGICK(timescale, r_uint<32>);
        MUCH_BLACKER_MAGICK(duration, r_arg_bits<uint64_t>);
        MUCH_BLACKER_MAGICK(language, r_charray<3, 5>);

        template<typename Reader>
        void read(Reader& r)
        {
            Base::read(r);
            size_t bitcount = (Base::version == 1) ? 64 : 32;
            creation_time << read_args(r, bitcount);
            modification_time << read_args(r, bitcount);
            timescale << r;
            duration << read_args(r, bitcount);

            r.skip(1); // padding
            language << r;

            r.skip(16); // reserved
        }
    };
}
