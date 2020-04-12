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

#include <bitcommons/file_byte_source.hpp>
#include <stdexcept>

using namespace bitcommons;

//----------------------------------------------------------------------
file_byte_source::file_byte_source(std::shared_ptr<file_reader> reader)
        : _reader(std::move(reader)),
          _buffer(shared_buffer::allocate(32 * 1024)) // TODO: Magic number, replace it
        , _position(0), _last(0) {

}

//----------------------------------------------------------------------
size_t file_byte_source::get_n(uint64_t& buf, size_t bytes) {
    if (
            _position < _last ||
            _position + bytes >= _last + _buffer.size() ||
            _buffer.size() == 0)
    {
        load_buffer();
    }

    if (_buffer.size() == 0 && available() == 0) {
        throw std::runtime_error("Cannot read beyond the end of the file");
    }

    auto to_shift = std::min(available(), bytes);
    for (size_t iter = 0; iter < to_shift; ++iter) {
        buf <<= 8;
        buf |= _buffer.get()[_position - _last];
        ++_position;
    }

    return to_shift;
}

//----------------------------------------------------------------------
bool file_byte_source::depleted() {
    return _reader->depleted();
}

//----------------------------------------------------------------------
uint64_t file_byte_source::available() {
    return _reader->size() - _position;
}

//----------------------------------------------------------------------
uint64_t file_byte_source::position() {
    return _position;
}

//----------------------------------------------------------------------
void file_byte_source::seek(uint64_t position) {
    if (position > _reader->size()) {
        throw std::range_error("Cannot seek beyond file size");
    }

    _position = position;
}

//----------------------------------------------------------------------
void file_byte_source::skip(uint64_t bytes) {
    if (bytes > available()) {
        throw std::range_error("Cannot skip beyond file size");
    }

    _position += bytes;
}

//----------------------------------------------------------------------
void file_byte_source::load_buffer()
{
    auto read = _reader->read(_buffer.get(), _position, _buffer.capacity());
    _buffer.resize(read);
    _last = _position;
}

//----------------------------------------------------------------------
std::shared_ptr<file_byte_source> file_byte_source::clone()
{
    auto ret = std::make_shared<file_byte_source>(_reader->clone());
    ret->_buffer = shared_buffer::clone(_buffer);
    ret->_position = _position;
    ret->_last = _last;
    return ret;
}
