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
#include <unordered_map>
#include <string>
#include <utility>
#include <hrodvitnir/core/ce_crc64.hpp>
#include <hrodvitnir/core/fieldset.hpp>

namespace hrodvitnir::core
{
    //------------------------------------------------------------------------------
    // this is the one way to get this into a member, along with the name
    struct property_initializer
    {
        fieldset** host;
        const char* name;
        uint64_t key;
    };

    //------------------------------------------------------------------------------
    // base class, contains and index and some static name<->hash mapping
    struct property_base
    {
        using mapping_t = std::unordered_map<uint64_t, std::string>;

        uint8_t index = 0;

        //--------------------------------------------------------------------------
        static mapping_t& mapping()
        {
            static mapping_t map;
            return map;
        }

        //--------------------------------------------------------------------------
        static uint64_t register_name(uint64_t key, const char* name)
        {
            mapping()[key] = name;
            return key;
        }

        //--------------------------------------------------------------------------
        static const std::string& get_name(uint64_t key)
        {
            return mapping()[key];
        }

        //--------------------------------------------------------------------------
        fieldset* __get_target()
        {
            auto raw_ptr = reinterpret_cast<uint8_t*>(this);
            auto target_ptr = raw_ptr - index - sizeof(void*);
            auto result = reinterpret_cast<fieldset**>(target_ptr);
            return *result;
        }

        //--------------------------------------------------------------------------
        const fieldset* __get_target() const
        {
            return const_cast<property_base*>(this)->__get_target();
        }
    };

    //------------------------------------------------------------------------------
    // this one actually adds a name and registers the hash
    // its only raison d'etre is binding name to a hash in the name mapping
    template<uint64_t Hash>
    struct named_property: public property_base
    {
        named_property(const char* name)
        {
            static const auto dummy = static_cast<uint8_t>(property_base::register_name(Hash, name));
            if (dummy > 0) {}
        }
    };

    //------------------------------------------------------------------------------
    template<typename Reader, typename... Args>
    struct read_argument_helper
    {
        std::tuple<Reader*, Args...> params;

        //--------------------------------------------------------------------------
        read_argument_helper(Reader& r, Args&&... args)
            : params(&r, std::forward<Args>(args)...)
        {

        }

        //--------------------------------------------------------------------------
        // read the field from bitreader
        template<typename ReadSpec>
        typename ReadSpec::value_type apply()
        {
            auto applicator = []<typename R, typename... LArgs>(R* reader, LArgs&&... args) -> auto {
                return ReadSpec::template read(*reader, std::forward<LArgs>(args)...);
            };
            return std::apply(applicator, params);
        }
    };

    //------------------------------------------------------------------------------
    // convenience wrapper to use in parser code
    // field << read_args(reader, Base::version) for example
    template<typename Reader, typename... Args>
    read_argument_helper<Reader, Args...> read_args(Reader& r, Args&&... params)
    {
        return read_argument_helper<Reader, Args...>(r, std::forward<Args>(params)...);
    }

    //------------------------------------------------------------------------------
    // now that's the real meat -- field binding
    // it does some ugly shit pointer magic inside to get to the fieldset
    // while only keeping an unsigned byte inside, using it as an index to infer offset
    // to the fieldset pointer somewhere at the bottom of the binding type
    template<typename ReadSpec, uint64_t Hash>
    struct read_mapped_property: public named_property<Hash>
    {
        using value_type = typename ReadSpec::value_type;
        using read_spec = ReadSpec;

        //--------------------------------------------------------------------------
        read_mapped_property(property_initializer&& init): named_property<Hash>(init.name)
        {
            auto raw_ptr = reinterpret_cast<uint8_t*>(this);
            auto begin_ptr = reinterpret_cast<uint8_t*>(init.host) + sizeof(void*);
            auto diff = raw_ptr - begin_ptr;
            this->index = diff;
        }

        //--------------------------------------------------------------------------
        read_mapped_property<ReadSpec, Hash>& operator=(const value_type& val)
        {
            this->__get_target()->set(Hash, val);
            return *this;
        }

        //--------------------------------------------------------------------------
        const std::string& get_name() const
        {
            return property_base::get_name(Hash);
        }

        //--------------------------------------------------------------------------
        operator const value_type&() const
        {
            return std::any_cast<const value_type&>(this->__get_target()->get(Hash));
        }

        //--------------------------------------------------------------------------
        operator value_type&()
        {
            return std::any_cast<value_type&>(this->__get_target()->get(Hash));
        }

        //--------------------------------------------------------------------------
        // read the field from bitreader
        template<typename Reader>
        void operator<<(Reader& r)
        {
            *this = ReadSpec::template read<Reader>(r);
        }

        //--------------------------------------------------------------------------
        template<typename Reader, typename... Args>
        void operator<<(read_argument_helper<Reader, Args...>&& helper)
        {
            *this = helper.template apply<ReadSpec>();
        }

        //--------------------------------------------------------------------------
        // sometimes operator value_type&() ain't enough to hint the compiler
        value_type& operator()()
        {
            return *this;
        }

        //--------------------------------------------------------------------------
        const value_type& operator()() const
        {
            return *this;
        }
    };
}

#define MUCH_BLACKER_MAGICK(Prop, ...) read_mapped_property<__VA_ARGS__, #Prop ## _crc64> Prop = property_initializer{&this->__box, #Prop, Base::ThisClass::register_property(#Prop ## _crc64)}
