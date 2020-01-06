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
#include <hrodvitnir/core/box-binding.hpp>
#include <hrodvitnir/core/boxes/box.hpp>
#include <hrodvitnir/core/boxes/file-type.hpp>
#include <hrodvitnir/core/boxes/full-box.hpp>
#include <hrodvitnir/core/boxes/skipper-box.hpp>
#include <hrodvitnir/core/boxes/movie-header-box.hpp>
#include <hrodvitnir/core/boxes/track-header-box.hpp>
#include <hrodvitnir/core/boxes/media-header-box.hpp>
#include <hrodvitnir/core/boxes/handler-box.hpp>
#include <hrodvitnir/core/boxes/video-media-header-box.hpp>
#include <hrodvitnir/core/boxes/sound-media-header-box.hpp>
#include <hrodvitnir/core/boxes/sample-description-box.hpp>
#include <hrodvitnir/core/boxes/sample-entry-box.hpp>
#include <hrodvitnir/core/boxes/time-to-sample-box.hpp>
#include <hrodvitnir/core/boxes/composition-offset-box.hpp>
#include <hrodvitnir/core/boxes/sync-sample-box.hpp>
#include <hrodvitnir/core/boxes/chunk-offset-box.hpp>
#include <hrodvitnir/core/boxes/sample-to-chunk-box.hpp>
#include <hrodvitnir/core/boxes/sample-size-box.hpp>
#include <hrodvitnir/core/boxes/sample-dependency-type-box.hpp>
#include <hrodvitnir/core/boxes/visual-sample-entry-box.hpp>

#include <hrodvitnir/core/boxes/pixel-aspect-ratio-box.hpp>
#include <hrodvitnir/core/boxes/clean-aperture-box.hpp>
#include <hrodvitnir/core/boxes/colour-information-box.hpp>

#include <hrodvitnir/core/boxes/edit-list-box.hpp>

namespace hrodvitnir::core::boxes
{
    template<template<typename Base> class... Args>
    using data_box_binding_t = box_binding_t<box, Args..., skipper>;

    template<template<typename Base> class... Args>
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

    using pasp_box_t = data_box_binding_t<pixel_aspect_ratio>;
    using clap_box_t = data_box_binding_t<clean_aperture>;
    using colr_box_t = data_box_binding_t<colour_information>;

    using elst_box_t = data_box_binding_t<full_box, edit_list>;
}
