/* BSD 3-Clause License

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

#include <bitcommons/memory_byte_source.hpp>

using namespace bitcommons;

//----------------------------------------------------------------------
memory_byte_source::memory_byte_source()
{
    _current = nullptr;
}

//----------------------------------------------------------------------
memory_byte_source::memory_byte_source(const uint8_t* data, size_t size)
        : _data(shared_buffer::copy_mem(data, size))
{
    _current = _data.begin();
}

//----------------------------------------------------------------------
size_t memory_byte_source::get_n(uint64_t& buf, size_t bytes)
{
    if (_current == _data.end()) {
        throw std::runtime_error("Access beyond data buffer boundaries");
    }

    auto to_shift = std::min(bytes, available());
    for (size_t iter = 0; iter < to_shift; ++iter) {
        buf <<= 8;
        buf |= *_current;
        ++_current;
    }

    return to_shift;
}

//----------------------------------------------------------------------
bool memory_byte_source::depleted()
{
    return true; // Memory buffer source is always depleted
    // I.e. never gets any additional data
}

//----------------------------------------------------------------------
uint64_t memory_byte_source::available()
{
    return static_cast<uint64_t>(_data.end() - _current);
}

//----------------------------------------------------------------------
uint64_t memory_byte_source::position()
{
    return static_cast<uint64_t>(_current - _data.begin());
}

//----------------------------------------------------------------------
void memory_byte_source::seek(uint64_t position)
{
    if (position > _data.size()) {
        throw std::range_error("Position outside of the data buffer");
    }

    _current = _data.begin() + position;
}

//----------------------------------------------------------------------
void memory_byte_source::skip(uint64_t bytes)
{
    if (bytes > available()) {
        throw std::range_error("Cannot skip beyond the boundaries of the data buffer");
    }

    _current += bytes;
}

//----------------------------------------------------------------------
std::shared_ptr<memory_byte_source> memory_byte_source::clone()
{
    auto ret = std::make_shared<memory_byte_source>();
    ret->_data = shared_buffer::clone(_data);
    ret->_current = ret->_data.begin() + position();
    return ret;
}
