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
#include <memory>
#include <map>

namespace hrodvitnir::core
{
    class field_ordering_map
    {
    public:
        template<typename BoxBinding>
        static std::shared_ptr<field_ordering_map> get_instance()
        {
            static std::shared_ptr<field_ordering_map> instance;
            if (!instance) {
                instance = std::make_shared<field_ordering_map>();
            }
            return instance;
        }

        using internal_type = std::map<size_t, uint64_t>;
        using const_iterator = internal_type::const_iterator;

        const_iterator begin() const { return _field_order.cbegin(); }
        const_iterator cbegin() const { return _field_order.cbegin(); }
        const_iterator end() const { return _field_order.cend(); }
        const_iterator cend() const { return _field_order.cend(); }

        void register_field(uint64_t key)
        {
            _field_order[_index] = key;
            ++_index;
        }

    private:
        std::map<uint64_t, size_t> _field_order;
        size_t _index = 0;
    };
}