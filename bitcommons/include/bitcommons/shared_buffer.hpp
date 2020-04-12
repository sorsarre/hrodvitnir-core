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
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <memory>

namespace bitcommons
{
    //--------------------------------------------------------------------------
    class shared_buffer
    {
    public:
        //----------------------------------------------------------------------
        shared_buffer();

        //----------------------------------------------------------------------
        shared_buffer(shared_buffer&& other) = default;
        shared_buffer(const shared_buffer& other) = default;
        shared_buffer& operator=(const shared_buffer& other) = default;
        ~shared_buffer() = default;

        //----------------------------------------------------------------------
        using iterator = uint8_t*;
        using const_iterator = const uint8_t*;

        size_t size() const { return _state->size; }
        size_t capacity() const { return _state->capacity; }
        const uint8_t* get() const { return _state->data; }
        uint8_t* get() { return _state->data; }

        iterator begin() { return get(); }
        iterator end() { return get() + size(); }
        const_iterator begin() const { return get(); }
        const_iterator end() const { return get() + size(); }
        const_iterator cbegin() const { return get(); }
        const_iterator cend() const { return get() + size(); }

        //----------------------------------------------------------------------
        static shared_buffer wrap_mem(uint8_t* data, size_t size);
        static shared_buffer copy_mem(const uint8_t* data, size_t size);
        static shared_buffer clone(const shared_buffer& buffer);
        static shared_buffer allocate(size_t size);
        void realloc(size_t new_size);
        void resize(size_t new_size);
        operator bool() const;
        uint8_t operator[](size_t index) const { return get()[index]; }

    private:
        //----------------------------------------------------------------------
        struct _internal
        {
            uint8_t* data = nullptr;
            size_t capacity = 0;
            size_t size = 0;
        };

        //----------------------------------------------------------------------
        static std::shared_ptr<_internal> make_internal(_internal state);
        static std::shared_ptr<_internal> make_internal(
                uint8_t* data,
                size_t size,
                size_t capacity);

        explicit shared_buffer(_internal state);
        explicit shared_buffer(std::shared_ptr<_internal> state);

        std::shared_ptr<_internal> _state;
    };

    //--------------------------------------------------------------------------
    template<typename T>
    shared_buffer wrap_object_into_buffer(T* obj)
    {
        auto obj_data = reinterpret_cast<uint8_t*>(obj);
        return shared_buffer::wrap_mem(obj_data, sizeof(*obj));
    }

    //--------------------------------------------------------------------------
    template<typename T>
    const T* shared_buffer_cast(const shared_buffer& buf)
    {
        return reinterpret_cast<const T*>(buf.get());
    }
}
