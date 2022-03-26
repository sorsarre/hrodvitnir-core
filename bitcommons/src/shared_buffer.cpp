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

#include <bitcommons/shared_buffer.hpp>

using namespace bitcommons;

//----------------------------------------------------------------------
shared_buffer::shared_buffer() : _state(make_internal(_internal{})) {}

//----------------------------------------------------------------------
shared_buffer shared_buffer::wrap_mem(uint8_t* data, size_t size)
{
    return shared_buffer(make_internal(data, size, size));
}

//----------------------------------------------------------------------
shared_buffer shared_buffer::copy_mem(const uint8_t* data, size_t size)
{
    auto data_copy = new uint8_t[size];
    std::copy(data, data + size, data_copy);
    return shared_buffer(make_internal(data_copy, size, size));
}

//----------------------------------------------------------------------
shared_buffer shared_buffer::clone(const shared_buffer& buffer)
{
    if (!buffer.get())
    {
        return shared_buffer(_internal{});
    }

    auto state = make_internal(new uint8_t[buffer.capacity()], buffer.size(), buffer.capacity());

    std::copy(buffer.get(), buffer.get() + buffer.size(), state->data);
    return shared_buffer(state);
}

//----------------------------------------------------------------------
shared_buffer shared_buffer::allocate(size_t size)
{
    if (size == 0)
    {
        return shared_buffer();
    }
    else
    {
        return shared_buffer(make_internal(new uint8_t[size], 0, size));
    }
}

//----------------------------------------------------------------------
void shared_buffer::realloc(size_t new_size)
{
    if (new_size == capacity())
    {
        return;
    }
    else if (new_size == 0)
    {
        _state = make_internal(_internal{});
    }
    else
    {
        auto new_data = new uint8_t[new_size];
        auto copy_size = std::min(size(), new_size);
        if (copy_size > 0)
        {
            std::copy(begin(), begin() + copy_size, new_data);
        }

        _state = make_internal(new_data, copy_size, new_size);
    }
}

//----------------------------------------------------------------------
void shared_buffer::resize(size_t new_size)
{
    if (new_size == size())
    {
        return;
    }
    else if (new_size <= capacity())
    {
        _state->size = new_size;
    }
    else
    {
        auto new_data = new uint8_t[new_size];
        auto copy_size = std::min(new_size, size());
        if (copy_size > 0)
        {
            std::copy(begin(), begin() + copy_size, new_data);
        }

        _state = make_internal(new_data, new_size, new_size);
    }
}

//----------------------------------------------------------------------
shared_buffer::operator bool() const
{
    return (_state->data != nullptr);
}

//----------------------------------------------------------------------
std::shared_ptr<shared_buffer::_internal> shared_buffer::make_internal(_internal state)
{
    std::shared_ptr<_internal> ret(new _internal(state),
                                   [](_internal* ptr)
                                   {
                                       delete[] ptr->data;
                                       ptr->size = 0;
                                       ptr->capacity = 0;
                                   });

    return ret;
}

//----------------------------------------------------------------------
std::shared_ptr<shared_buffer::_internal> shared_buffer::make_internal(uint8_t* data, size_t size, size_t capacity)
{
    _internal state;
    state.data = data;
    state.size = size;
    state.capacity = capacity;
    return make_internal(state);
}

//----------------------------------------------------------------------
shared_buffer::shared_buffer(_internal state) : _state(make_internal(state)) {}

//----------------------------------------------------------------------
shared_buffer::shared_buffer(std::shared_ptr<_internal> state) : _state(std::move(state)) {}
