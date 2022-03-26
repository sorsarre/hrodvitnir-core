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

namespace hrodvitnir::core::boxes
{
struct edit_list_item
{
    uint64_t segment_duration;
    int64_t media_time;
    int16_t media_rate_integer; // TODO: replace with Q16.16
    int16_t media_rate_fraction;
};

struct r_elst_item
{
    using value_type = edit_list_item;

    template <typename Reader>
    static value_type read(Reader& r, uint32_t version)
    {
        value_type ret;
        size_t bitcount = (version == 1) ? 64 : 32;
        r.read(bitcount, ret.segment_duration);
        r.read(bitcount, ret.media_time);
        r.read(16, ret.media_rate_integer);
        r.read(16, ret.media_rate_fraction);
        return ret;
    }
};

template <typename Base>
struct edit_list : public Base
{
    MUCH_BLACKER_MAGICK(entry_count, r_uint<32>);
    MUCH_BLACKER_MAGICK(entries, r_arg_items_arg<std::vector, r_elst_item>);

    template <typename Reader>
    void read(Reader& r)
    {
        Base::read(r);
        entry_count << r;
        entries << read_args(r, entry_count(), Base::version());
    }
};
} // namespace hrodvitnir::core::boxes
