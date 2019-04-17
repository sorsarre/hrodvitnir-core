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
#include <cstdint>
#include <cstddef>
#include <hrodvitnir/core/data-source.hpp>

namespace hrodvitnir::core
{
    template<typename Original>
    class data_source_wrapper: public data_source
    {
    public:
        data_source_wrapper(const std::shared_ptr<Original>& orig): _orig(orig)
        {

        }

        size_t get_n(uint64_t& buf, size_t bytes) override
        {
            return _orig->get_n(buf, bytes);
        }

        bool depleted() override
        {
            return _orig->depleted();
        }

        uint64_t available() override
        {
            return _orig->available();
        }

        uint64_t position() override
        {
            return _orig->position();
        }

        void seek(uint64_t position) override
        {
            _orig->seek(position);
        }

        void skip(uint64_t bytes) override
        {
            _orig->skip(bytes);
        }

        std::shared_ptr<data_source> clone() override
        {
            auto orig_clone = _orig->clone();
            return std::make_shared<data_source_wrapper<Original>>(orig_clone);
        }

    private:
        std::shared_ptr<Original> _orig;
    };

    template<typename Original>
    std::shared_ptr<data_source_wrapper<Original>> wrap_data_source(const std::shared_ptr<Original>& orig)
    {
        return std::make_shared<data_source_wrapper<Original>>(orig);
    }
}
