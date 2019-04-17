/* BSD 3-Clause License

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
#include <bitcommons/file_reader.hpp>
#include <bitcommons/shared_buffer.hpp>

using namespace bitcommons;

namespace {
    //--------------------------------------------------------------------------
    uint8_t* generate_test_data(size_t size)
    {
        if (size == 0) {
            return nullptr;
        }

        auto ret = new uint8_t[size];
        for (size_t iter = 0; iter < size; ++iter) {
            ret[iter] = static_cast<uint8_t>(iter+1);
        }
        return ret;
    }

    //--------------------------------------------------------------------------
    class fake_file_reader: public file_reader
    {
    public:
        //----------------------------------------------------------------------
        explicit fake_file_reader(size_t size)
        {
            _data = shared_buffer::wrap_mem(
                generate_test_data(size),
                size
            );
        }

        //----------------------------------------------------------------------
        size_t read(uint8_t* dest, uint64_t position, size_t bytes) override
        {
            if (position > _data.size()) {
                throw std::runtime_error("Cannot seek beyond the end of the buffer");
            }

            size_t to_copy = std::min(_data.size() - position, bytes);
            auto begin = _data.begin() + position;
            auto end = begin + to_copy;
            std::copy(begin, end, dest);
            return to_copy;
        }

        //----------------------------------------------------------------------
        uint64_t size() override
        {
            return _data.size();
        }

        //----------------------------------------------------------------------
        std::shared_ptr<file_reader> clone() override
        {
            auto ret = new fake_file_reader(*this);
            return std::shared_ptr<file_reader>(ret);
        }

        //----------------------------------------------------------------------
        bool depleted() override
        {
            return true;
        }

        //----------------------------------------------------------------------
        ~fake_file_reader() override = default;

    private:
        fake_file_reader(const fake_file_reader& other)
        {
            _data = shared_buffer::clone(other._data);
        }

        shared_buffer _data;
    };
}
