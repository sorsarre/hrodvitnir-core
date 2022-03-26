// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/box-binding.hpp>
#include <hrodvitnir/core/boxes/avc-decoder-configuration-box.hpp>
#include <hrodvitnir/core/boxes/box.hpp>
#include <hrodvitnir/core/boxes/chunk-offset-box.hpp>
#include <hrodvitnir/core/boxes/clean-aperture-box.hpp>
#include <hrodvitnir/core/boxes/colour-information-box.hpp>
#include <hrodvitnir/core/boxes/composition-offset-box.hpp>
#include <hrodvitnir/core/boxes/edit-list-box.hpp>
#include <hrodvitnir/core/boxes/file-type.hpp>
#include <hrodvitnir/core/boxes/full-box.hpp>
#include <hrodvitnir/core/boxes/handler-box.hpp>
#include <hrodvitnir/core/boxes/media-header-box.hpp>
#include <hrodvitnir/core/boxes/movie-header-box.hpp>
#include <hrodvitnir/core/boxes/pixel-aspect-ratio-box.hpp>
#include <hrodvitnir/core/boxes/sample-dependency-type-box.hpp>
#include <hrodvitnir/core/boxes/sample-description-box.hpp>
#include <hrodvitnir/core/boxes/sample-entry-box.hpp>
#include <hrodvitnir/core/boxes/sample-size-box.hpp>
#include <hrodvitnir/core/boxes/sample-to-chunk-box.hpp>
#include <hrodvitnir/core/boxes/skipper-box.hpp>
#include <hrodvitnir/core/boxes/sound-media-header-box.hpp>
#include <hrodvitnir/core/boxes/sync-sample-box.hpp>
#include <hrodvitnir/core/boxes/time-to-sample-box.hpp>
#include <hrodvitnir/core/boxes/track-header-box.hpp>
#include <hrodvitnir/core/boxes/video-media-header-box.hpp>
#include <hrodvitnir/core/boxes/visual-sample-entry-box.hpp>

namespace hrodvitnir::core::boxes
{
template <template <typename Base> class... Args>
using data_box_binding_t = box_binding_t<box, Args..., skipper>;

template <template <typename Base> class... Args>
using cont_box_binding_t = box_binding_t<box, Args...>;

using default_box_t = data_box_binding_t<>;
using cont_box_t = cont_box_binding_t<>;
using full_box_t = data_box_binding_t<full_box>;
using full_cont_box_t = cont_box_binding_t<full_box>;
using ftyp_box_t = data_box_binding_t<file_type>;
using mvhd_box_t = data_box_binding_t<full_box, movie_header>;
using tkhd_box_t = data_box_binding_t<full_box, track_header>;
using mdhd_box_t = data_box_binding_t<full_box, media_header>;
using hdlr_box_t = data_box_binding_t<full_box, handler>;
using vmhd_box_t = data_box_binding_t<full_box, video_media_header>;
using smhd_box_t = data_box_binding_t<full_box, sound_media_header>;
using stsd_box_t = cont_box_binding_t<full_box, sample_description>;
using stts_box_t = data_box_binding_t<full_box, time_to_sample>;
using ctts_box_t = data_box_binding_t<full_box, composition_offset>;
using stss_box_t = data_box_binding_t<full_box, sync_sample>;
using stco_box_t = data_box_binding_t<full_box, chunk_offset>;
using stsc_box_t = data_box_binding_t<full_box, sample_to_chunk>;
using stsz_box_t = data_box_binding_t<full_box, sample_size>;
using sdtp_box_t = data_box_binding_t<full_box, sample_dependency_type>;

using sample_entry_box_t = data_box_binding_t<sample_entry>;
using visual_sample_entry_box_t = cont_box_binding_t<sample_entry, visual_sample_entry>;
using avcC_box_t = data_box_binding_t<avc_decoder_configuration>;

using pasp_box_t = data_box_binding_t<pixel_aspect_ratio>;
using clap_box_t = data_box_binding_t<clean_aperture>;
using colr_box_t = data_box_binding_t<colour_information>;

using elst_box_t = data_box_binding_t<full_box, edit_list>;
} // namespace hrodvitnir::core::boxes
