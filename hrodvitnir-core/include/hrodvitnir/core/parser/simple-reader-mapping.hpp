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
#include <map>
#include <hrodvitnir/core/parser/reader-mapping.hpp>
#include <hrodvitnir/core/box-defs.hpp>

namespace hrodvitnir::core
{
    //--------------------------------------------------------------------------
    class simple_reader_mapping: public reader_mapping
    {
    public:
        //----------------------------------------------------------------------
        simple_reader_mapping(const partial_reader_type& def_rd = &default_reader)
        {
            _default = def_rd;
        }

        //----------------------------------------------------------------------
        template<typename BoxBinding>
        void assign(const std::initializer_list<fourcc>& fcc)
        {
            for (const auto& f: fcc) {
                _registry.emplace(uuid::from_fourcc(f), &partial_read<BoxBinding>);
            }
        }

        //----------------------------------------------------------------------
        const partial_reader_type& get(const uuid& u) const override
        {
            auto iter = _registry.find(u);
            if (iter != _registry.end()) {
                return iter->second;
            } else {
                return _default;
            }
        }

    private:
        //----------------------------------------------------------------------
        template<typename BoxBinding>
        static void partial_read(hrodvitnir::fieldset& fs, reader_type& reader)
        {
            BoxBinding box{fs};
            box.read(reader);
        }

        //----------------------------------------------------------------------
        static void default_reader(fieldset& fs, reader_type& r)
        {
            boxes::default_box_t{fs}.read(r);
        }

        //----------------------------------------------------------------------
        std::map<hrodvitnir::uuid, partial_reader_type> _registry;
        partial_reader_type _default;
    };
}
