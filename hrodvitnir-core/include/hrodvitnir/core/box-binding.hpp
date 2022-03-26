/*
BSD 3-Clause License

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
#include <cstddef>
#include <hrodvitnir/core/field-ordering.hpp>
#include <hrodvitnir/core/fieldset.hpp>
#include <hrodvitnir/core/uuid.hpp>

namespace hrodvitnir::core
{
//------------------------------------------------------------------------------
// HELL BEGINS (BOX BINDINGS OVER FIELDSET)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
struct binding_base
{
    using ThisClass = binding_base;

    static uint64_t register_property(uint64_t key)
    {
        return key;
    }

    static std::shared_ptr<field_ordering_map> get_ordering()
    {
        return nullptr;
    }

    fieldset* __box;
};

//------------------------------------------------------------------------------
template <typename T>
struct ordered_binding_base
{
    using ThisClass = ordered_binding_base<T>;

    static uint64_t register_property(uint64_t key)
    {
        static bool finished = false;
        if (key == 0)
        {
            finished = true;
        }
        else
        {
            if (!finished)
            {
                auto registry = field_ordering_map::get_instance<T>();
                registry->register_field(key);
            }
        }
        return key;
    }
    static std::shared_ptr<field_ordering_map> get_ordering()
    {
        return field_ordering_map::get_instance<T>();
    }

    fieldset* __box;
};

//------------------------------------------------------------------------------
/*struct binding_end
{
    struct{} __dummy;
};*/

//------------------------------------------------------------------------------
// tells how many fields there are in the binding type
// gotta blaze them straight in the face if ther are more than 256
// as indexing would wrap over and fail then
/*template<typename A, typename F>
constexpr ptrdiff_t span()
{
    A a;
    auto ptr_end = &binding_end::__dummy;
    auto ptr_beacon = &binding_base::__box;
    auto ptr_b = reinterpret_cast<const uint8_t*>(&(a.*ptr_end));
    auto ptr_c = reinterpret_cast<const uint8_t*>(&(a.*ptr_beacon));
    auto align_beacon = alignof(decltype(binding_base::__box));
    auto align_field = alignof(F);
    auto align = (align_beacon < align_field) ? align_field : align_beacon;
    return (ptr_b - ptr_c - align) / sizeof(F);
}*/
// NODELETE: could be useful for testing and failing gracefully upon index blowing up beyond 255

//------------------------------------------------------------------------------
// small helper which gets latched onto the inheritance sequence last
template <typename Acc>
struct finish_helper_t : Acc
{
    finish_helper_t(fieldset& fs)
    {
        fs._ordering = Acc::ThisClass::get_ordering();
        this->__box = &fs;
        Acc::ThisClass::register_property(0);
    }

    finish_helper_t()
    {
        this->__box = nullptr;
    }
};

//------------------------------------------------------------------------------
// just a little hardcore TMP crap, minding its own business, mending a primus
template <typename Acc, template <typename Base> class First, template <typename Base> class... Args>
struct box_binding_helper
{
    using type = typename box_binding_helper<First<Acc>, Args...>::type;
};

//------------------------------------------------------------------------------
// go on, nothing of note here, just latching the last piece over the heap
template <typename Acc, template <typename Base> class First>
struct box_binding_helper<Acc, First>
{
    using type = finish_helper_t<First<Acc>>;
};

//------------------------------------------------------------------------------
// this one pieces them all together
// kinda redundant, might as well do the same in the following typedef
template <template <typename Base> class... Parts>
struct box_binding
{
    using intermediate_type = typename box_binding_helper<binding_base, Parts...>::type;
    using type = typename box_binding_helper<ordered_binding_base<intermediate_type>, Parts...>::type;
};

//------------------------------------------------------------------------------
// define a box binding out of parts
template <template <typename Base> class... Parts>
using box_binding_t = typename box_binding<Parts...>::type;

} // namespace hrodvitnir::core
