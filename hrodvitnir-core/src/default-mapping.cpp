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

#include <hrodvitnir/core/parser/default-mapping.hpp>
#include <hrodvitnir/core/parser/simple-reader-mapping.hpp>
#include <hrodvitnir/core/box-defs.hpp>

namespace hrodvitnir::core
{
    const reader_mapping::ptr get_base_mapping()
    {
        static std::shared_ptr<simple_reader_mapping> mapping;

        if (!mapping) {
            mapping = std::make_shared<simple_reader_mapping>();

            mapping->assign<core::boxes::cont_box_t>({
                "moov", "trak", "mdia", "stbl",
                "minf", "tref", "edts", "dinf",
                "mvex", "moof", "traf", "mfra",
                "udta", "meco", "sinf", "schi",
                "paen", "strk", "strd", "rinf",
                "cinf", "hnti", "hinf", "fdsa"
                });
            mapping->assign<core::boxes::full_cont_box_t>({
                "meta", // FIXME: DANGER DANGER DANGER BACKTRACKING PARSER NECESSARY
                "iref", // FIXME: DYNAMIC MAPPING RECONFIG NECESSARY HERE
            });
            mapping->assign<core::boxes::full_box_t>({
                "nmhd", "sthd"
            });
            mapping->assign<core::boxes::ftyp_box_t>({"ftyp"});
            mapping->assign<core::boxes::mvhd_box_t>({"mvhd"});
            mapping->assign<core::boxes::tkhd_box_t>({"tkhd"});
            mapping->assign<core::boxes::mdhd_box_t>({"mdhd"});
            mapping->assign<core::boxes::hdlr_box_t>({"hdlr"});
            mapping->assign<core::boxes::vmhd_box_t>({"vmhd"});
            mapping->assign<core::boxes::smhd_box_t>({"smhd"});
            mapping->assign<core::boxes::stsd_box_t>({"stsd"});
            mapping->assign<core::boxes::visual_sample_entry_box_t>({
                "mp4v", "avc1"
            });
            mapping->assign<core::boxes::avcC_box_t>({"avcC"});

            mapping->assign<core::boxes::stts_box_t>({"stts"});
            mapping->assign<core::boxes::ctts_box_t>({"ctts"});
            mapping->assign<core::boxes::stss_box_t>({"stss"});
            mapping->assign<core::boxes::stco_box_t>({"stco"});
            mapping->assign<core::boxes::stsc_box_t>({"stsc"});
            mapping->assign<core::boxes::stsz_box_t>({"stsz"});
            mapping->assign<core::boxes::sdtp_box_t>({"sdtp"});

            mapping->assign<core::boxes::pasp_box_t>({"pasp"});
            mapping->assign<core::boxes::clap_box_t>({"clap"});
            mapping->assign<core::boxes::colr_box_t>({"colr"});

            mapping->assign<core::boxes::elst_box_t>({"elst"});
        }
        return mapping;
    }
}