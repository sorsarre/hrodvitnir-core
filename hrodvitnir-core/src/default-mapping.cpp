// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/box-defs.hpp>
#include <hrodvitnir/core/parser/default-mapping.hpp>
#include <hrodvitnir/core/parser/simple-reader-mapping.hpp>

namespace hrodvitnir::core
{
const reader_mapping::ptr get_base_mapping()
{
    static std::shared_ptr<simple_reader_mapping> mapping;

    if (!mapping)
    {
        mapping = std::make_shared<simple_reader_mapping>();

        mapping->assign<core::boxes::cont_box_t>({"moov", "trak", "mdia", "stbl", "minf", "tref", "edts", "dinf",
                                                  "mvex", "moof", "traf", "mfra", "udta", "meco", "sinf", "schi",
                                                  "paen", "strk", "strd", "rinf", "cinf", "hnti", "hinf", "fdsa"});
        mapping->assign<core::boxes::full_cont_box_t>({
            "meta", // FIXME: DANGER DANGER DANGER BACKTRACKING PARSER NECESSARY
            "iref", // FIXME: DYNAMIC MAPPING RECONFIG NECESSARY HERE
        });
        mapping->assign<core::boxes::full_box_t>({"nmhd", "sthd"});
        mapping->assign<core::boxes::ftyp_box_t>({"ftyp"});
        mapping->assign<core::boxes::mvhd_box_t>({"mvhd"});
        mapping->assign<core::boxes::tkhd_box_t>({"tkhd"});
        mapping->assign<core::boxes::mdhd_box_t>({"mdhd"});
        mapping->assign<core::boxes::hdlr_box_t>({"hdlr"});
        mapping->assign<core::boxes::vmhd_box_t>({"vmhd"});
        mapping->assign<core::boxes::smhd_box_t>({"smhd"});
        mapping->assign<core::boxes::stsd_box_t>({"stsd"});
        mapping->assign<core::boxes::visual_sample_entry_box_t>({"mp4v", "avc1"});
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
} // namespace hrodvitnir::core