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

#pragma once
#include <bitcommons/bitreader-utils.hpp>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>
#include <stdexcept>

namespace bitcommons
{
//--------------------------------------------------------------------------
template <typename Sink>
class bitwriter
{
  public:
    //----------------------------------------------------------------------
    bitwriter(std::shared_ptr<Sink> sink)
    {
        _state.sink = sink;
        _state.reset();
    }

    bitwriter(const bitwriter&) = delete;
    bitwriter& operator=(const bitwriter&) = delete;

    //----------------------------------------------------------------------
    size_t position() const
    {
        return _position(_state);
    }

    //----------------------------------------------------------------------
    void align(size_t bits)
    {
        _align(_state, bits);
    }

    //----------------------------------------------------------------------
    void skip(size_t bits)
    {
        _skip(_state, bits);
    }

    //----------------------------------------------------------------------
    void flush()
    {
        if ((internal_state::buffer_size - _state.avail) % 8 != 0)
        {
            _state.flush();
        }
    }

    //----------------------------------------------------------------------
    template <typename T>
    if_integral_void<T> write(T data, size_t bits)
    {
        size_t written = 0;
        size_t to_write = bits;

        while (written < bits)
        {
            size_t post = std::min<uint64_t>(_state.avail, to_write);
            T portion = (data >> (bits - written - post)) & _mask<T>(post);
            size_t diff = _state.avail - post;
            _state.buffer |= portion << diff;
            _state.avail -= post;

            if (_state.avail == 0)
            {
                _state.flush();
            }

            written += post;
            to_write -= post;
        }
    }

    //----------------------------------------------------------------------
    template <typename T>
    if_enum_void<T> write(T data, size_t bits)
    {
        using data_t = std::underlying_type_t<T>;
        auto value = static_cast<data_t>(data);
        this->write(value, bits);
    }

    //----------------------------------------------------------------------
    template <typename T>
    if_binary_codec_void<T> write(typename T::value_type data)
    {
        T::write(*this, data);
    }

  private:
    //----------------------------------------------------------------------
    struct internal_state
    {
        using buffer_type = uint8_t;
        static constexpr const size_t buffer_size = 8 * sizeof(buffer_type);

        std::shared_ptr<Sink> sink;
        buffer_type buffer;
        size_t avail;

        void reset()
        {
            buffer = buffer_type(0);
            avail = buffer_size;
        }

        void flush()
        {
            sink->put(buffer, buffer_size - avail);
            this->reset();
        }
    };

    //----------------------------------------------------------------------
    template <typename T>
    static constexpr T _mask(size_t bits)
    {
        return (bits == sizeof(T) * 8) ? (~0) : ((T(1) << bits) - 1);
    }

    //----------------------------------------------------------------------
    template <typename T>
    static constexpr size_t _bufsize(const T& buf)
    {
        return sizeof(T) * 8;
    }

    //----------------------------------------------------------------------
    size_t _position(const internal_state& state) const
    {
        return state.sink->position() * 8 + internal_state::buffer_size - state.avail;
    }

    //----------------------------------------------------------------------
    void _skip(internal_state& state, size_t bits) const
    {
        if (bits < state.avail)
        {
            state.avail -= bits;
        }
        else if (bits == state.avail)
        {
            state.avail = 0;
            state.flush();
        }
        else
        {
            size_t to_skip = bits;
            to_skip = bits - state.avail;
            state.avail = 0;
            state.flush();

            while (to_skip >= internal_state::buffer_size)
            {
                to_skip -= internal_state::buffer_size;
                state.avail = 0;
                state.flush();
            }

            if (to_skip > 0)
            {
                state.avail -= to_skip;
                to_skip = 0;
            }
        }
    }

    //----------------------------------------------------------------------
    void _align(internal_state& state, size_t bits) const
    {
        size_t advance = (bits - _position(state) % bits) % bits;
        if (advance > 0)
        {
            _skip(state, advance);
        }
    }

    internal_state _state;
};
} // namespace bitcommons
