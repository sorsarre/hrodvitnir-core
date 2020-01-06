/*
BSD 3-Clause License

Copyright (c) 2020, Andrey Kaydalov
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
#include <hrodvitnir/core/table.hpp>
#include <hrodvitnir/core/readspec.hpp>

namespace hrodvitnir::core::boxes
{
    template<typename Base>
    struct colour_information: public Base
    {
        MUCH_BLACKER_MAGICK(colour_type, r_fourcc);

        // nclx
        MUCH_BLACKER_MAGICK(colour_primaries, r_uint<16>);
        MUCH_BLACKER_MAGICK(transfer_characteristics, r_uint<16>);
        MUCH_BLACKER_MAGICK(matrix_coefficients, r_uint<16>);
        MUCH_BLACKER_MAGICK(full_range_flag, r_uint<7>);
        MUCH_BLACKER_MAGICK(reserved, r_uint<7>);

        // nclc
        MUCH_BLACKER_MAGICK(primaries_index, r_uint<16>);
        MUCH_BLACKER_MAGICK(transfer_function_index, r_uint<16>);
        MUCH_BLACKER_MAGICK(matrix_index, r_uint<16>);

        template<typename Reader>
        void read(Reader& r)
        {
            Base::read(r);
            colour_type << r;
            if (colour_type() == "nclx") {
                colour_primaries << r;
                transfer_characteristics << r;
                matrix_coefficients << r;
                full_range_flag << r;
                reserved << r;
            } else if (colour_type() == "nclc") {
                primaries_index << r;
                transfer_function_index << r;
                matrix_index << r;
            }
        }
    };
}
