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
    struct sample_dependency_type_entry
    {
        uint8_t is_leading: 2;
        uint8_t sample_depends_on: 2;
        uint8_t sample_is_depended_on: 2;
        uint8_t sample_has_redundancy: 2;
    };

    template<typename Base>
    struct sample_dependency_type: public Base
    {
        using r_sdtp_entry = r_lambda<[](auto& r) -> auto {
            using r_uint2 = r_uint<2>;
            sample_dependency_type_entry ret;
            ret.is_leading = r_uint2::read(r);
            ret.sample_depends_on = r_uint2::read(r);
            ret.sample_is_depended_on = r_uint2::read(r);
            ret.sample_has_redundancy = r_uint2::read(r);
            return ret;
        }>;

        using r_sdtp_table = r_filler_table<r_sdtp_entry>;

        MUCH_BLACKER_MAGICK(_contents, r_sdtp_table);

        template<typename Reader>
        void read(Reader& r)
        {
            Base::read(r);
            auto pos = r.position() / 8;
            auto rest = Base::box_end() - pos;
            _contents << read_args(r, rest);
        }
    };
}
