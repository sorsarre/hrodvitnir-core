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
#include <vector>
#include <bitcommons/codings/string-nullterm.hpp>
#include <hrodvitnir/core/table.hpp>
#include <hrodvitnir/core/uuid.hpp>
#include <hrodvitnir/core/fourcc.hpp>
#include <hrodvitnir/core/meta-helpers.hpp>
#include <hrodvitnir/core/bitreader-helpers.hpp>

namespace hrodvitnir::core
{
    //--------------------------------------------------------------------------
    template<size_t Bits>
    using uint_type_t = typename meta::pick_type<Bits, uint8_t, uint16_t, uint32_t, uint64_t>::type;

    //--------------------------------------------------------------------------
    template<size_t Bits>
    using int_type_t = typename meta::pick_type<Bits, int8_t, int16_t, int32_t, int64_t>::type;

    //--------------------------------------------------------------------------
    template<template<size_t> class Picker, size_t Bits>
    struct read_spec {
        using value_type = Picker<Bits>;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            return r.template read<value_type>(Bits);
        }
    };

    //--------------------------------------------------------------------------
    template<typename T>
    struct r_arg_bits {
        using value_type = T;

        template<typename Reader>
        static value_type read(Reader& r, size_t bits)
        {
            return r.template read<value_type>(bits);
        }
    };

    //--------------------------------------------------------------------------
    template<size_t bits> using r_uint = read_spec<uint_type_t, bits>;
    template<size_t bits> using r_bit = r_uint<bits>;
    template<size_t bits> using r_int = read_spec<int_type_t, bits>;

    //--------------------------------------------------------------------------
    template<size_t Size, typename ReadSpec>
    struct r_array {
        using element_type = typename ReadSpec::value_type;
        using value_type = std::vector<element_type>;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            value_type ret;
            for (size_t iter = 0; iter < Size; ++iter) {
                ret.push_back(ReadSpec::template read(r));
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------
    template<size_t Bits>
    struct r_char {
        static_assert(Bits <= 8, "Cannot read more than 8 bits into char!");

        using value_type = char;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            // read+cast combo to avoid sign extension for chars
            return static_cast<char>(r.template read<uint8_t>(Bits));
        }
    };

    //--------------------------------------------------------------------------
    template<size_t Size, size_t Bits>
    struct r_charray {
        using value_type = std::string;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            value_type ret;
            ret.reserve(Size);
            for (size_t iter = 0; iter < Size; ++iter) {
                ret.push_back(r_char<Bits>::read(r));
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------
    struct r_string {
        using value_type = std::string;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            return r.template read<bitcommons::ext::string_nullterm>();
        }
    };

    //--------------------------------------------------------------------------
    struct r_uuid {
        using value_type = uuid;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            uuid ret;
            brext::array_reader<uint8_t>::read(r, ret.begin(), 16, 8);
            return ret;
        }
    };
    
    //--------------------------------------------------------------------------
    template<typename Basis, unsigned int N, size_t Bits = sizeof(Basis)*8>
    struct r_qmn {
        using value_type = double;
        
        template<typename Reader>
        static value_type read(Reader& r)
        {
            auto raw = r.template read<Basis>(Bits);
            return static_cast<double>(raw) / (1 << N);
        }
    };

    //--------------------------------------------------------------------------
    struct r_transform {
        using value_type = std::vector<double>;
        
        template<typename Reader>
        static value_type read(Reader& r)
        {
            value_type ret;
            ret.reserve(9);
            for (size_t row = 0; row < 3; ++row) {
                auto a = r_qmn<int32_t, 16>::read(r);
                auto b = r_qmn<int32_t, 16>::read(r);
                auto c = r_qmn<int32_t, 30>::read(r);
                ret.push_back(a);
                ret.push_back(b);
                ret.push_back(c);
            }
            
            return ret;
        }
    };
    
    //--------------------------------------------------------------------------
    struct r_fourcc {
        using value_type = fourcc;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            auto value = r.template read<uint32_t>(32);
            return fourcc{value};
        }
    };

    //--------------------------------------------------------------------------
    template<typename T>
    struct r_manual {
        using value_type = T;
    };

    //--------------------------------------------------------------------------
    template<template<typename...> typename Container, typename ItemSpec>
    struct r_items_arg {
        using item_type = typename ItemSpec::value_type;
        using value_type = Container<item_type>;

        template<typename Reader>
        static value_type read(Reader& r, size_t count)
        {
            value_type ret;
            for (size_t iter = 0; iter < count; ++iter) {
                ret.emplace_back(ItemSpec::read(r));
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------
    template<typename EntrySpec>
    struct r_filler_table {
        using entry_type = typename EntrySpec::value_type;
        using value_type = std::shared_ptr<table<entry_type>>;

        template<typename Reader>
        static value_type read(Reader& r, size_t count)
        {
            auto ret = std::make_shared<array_table<entry_type>>();
            auto& entries = ret->entries();

            for (size_t iter = 0; iter < count; ++iter) {
                entries.emplace_back(EntrySpec::read(r));
            }

            return ret;
        }
    };

    //--------------------------------------------------------------------------
    template<template<typename...> typename Container, typename ItemSpec>
    struct r_arg_items_arg {
        using item_type = typename ItemSpec::value_type;
        using value_type = Container<item_type>;

        template<typename Reader, typename... Args>
        static value_type read(Reader& r, size_t count, Args&&... args)
        {
            value_type ret;
            for (size_t iter = 0; iter < count; ++iter) {
                ret.emplace_back(ItemSpec::read(r, std::forward<Args>(args)...));
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------
    template<template<typename...> typename Container, typename ItemSpec, size_t Size>
    struct r_items_fixed {
        using item_type = typename ItemSpec::value_Type;
        using value_type = Container<item_type>;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            value_type ret;
            for (size_t iter = 0; iter < Size; ++iter) {
                ret.emplace_back(ItemSpec::read(r));
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------
    template<typename EntrySpec, typename SizeSpec>
    struct r_fixed_table {
        using entry_type = typename EntrySpec::value_type;
        using value_type = std::shared_ptr<table<entry_type>>;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            auto size = SizeSpec::read(r);
            auto entry = std::move(EntrySpec::read(r));
            return std::make_shared<fixed_table<entry_type>>(std::move(entry), size);
        }
    };

    //--------------------------------------------------------------------------
    template<typename EntrySpec, typename SizeSpec>
    struct r_array_table {
        using entry_type = typename EntrySpec::value_type;
        using value_type = std::shared_ptr<table<entry_type>>;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            auto ret = std::make_shared<array_table<entry_type>>();
            auto size = SizeSpec::read(r);
            for (size_t iter = 0; iter < size; ++iter) {
                ret->entries().emplace_back(std::move(EntrySpec::read(r)));
            }
            return ret;
        }
    };

    //--------------------------------------------------------------------------
    template<typename Value, typename Lambda>
    struct r_lambda {
        using value_type = Value;

        template<typename Reader>
        static value_type read(Reader& r)
        {
            return Lambda{}(r);
        }
    };

}
