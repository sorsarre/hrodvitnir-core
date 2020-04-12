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
#include <hrodvitnir/core/table.hpp>
#include <hrodvitnir/core/readspec.hpp>

namespace hrodvitnir::core::boxes
{
    template<typename Base>
    struct avc_decoder_configuration: public Base
    {
        MUCH_BLACKER_MAGICK(configurationVersion, r_uint<8>);
        MUCH_BLACKER_MAGICK(AVCProfileIndication, r_uint<8>);
        MUCH_BLACKER_MAGICK(profile_compatibility, r_uint<8>);
        MUCH_BLACKER_MAGICK(AVCLevelIndication, r_uint<8>);
        MUCH_BLACKER_MAGICK(lengthSizeMinusOne, r_uint<2>);

        using r_sps = r_items_sized<std::vector, r_uint<8>, r_uint<16>>;
        using r_pps = r_sps;

        using r_sps_set = r_items_sized<std::vector, r_sps, r_uint<5>>;
        using r_pps_set = r_items_sized<std::vector, r_pps, r_uint<8>>;

        MUCH_BLACKER_MAGICK(sequenceParameterSets, r_sps_set);
        MUCH_BLACKER_MAGICK(pictureParameterSets, r_pps_set);

        template<typename Reader>
        void read(Reader& r)
        {
            Base::read(r);
            configurationVersion << r;
            AVCLevelIndication << r;
            profile_compatibility << r;
            AVCLevelIndication << r;
            r.skip(6);
            lengthSizeMinusOne << r;
            r.skip(3);
            sequenceParameterSets << r;
            pictureParameterSets << r;
        }
    };
}
