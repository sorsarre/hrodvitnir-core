// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#pragma once
#include <hrodvitnir/core/ce_crc64.hpp>
#include <hrodvitnir/core/fieldset.hpp>
#include <string>
#include <unordered_map>
#include <utility>

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
template <uint64_t Hash>
struct named_property : public property_base
{
    named_property(const char* name)
    {
        static const auto dummy = static_cast<uint8_t>(property_base::register_name(Hash, name));
        if (dummy > 0)
        {
        }
    }
};

//------------------------------------------------------------------------------
template <typename Reader, typename... Args>
struct read_argument_helper
{
    std::tuple<Reader*, Args...> params;

    //--------------------------------------------------------------------------
    read_argument_helper(Reader& r, Args&&... args) : params(&r, std::forward<Args>(args)...) {}

    //--------------------------------------------------------------------------
    // read the field from bitreader
    template <typename ReadSpec>
    typename ReadSpec::value_type apply()
    {
        auto applicator = []<typename R, typename... LArgs>(R * reader, LArgs && ... args)->auto
        {
            return ReadSpec::template read(*reader, std::forward<LArgs>(args)...);
        };
        return std::apply(applicator, params);
    }
};

//------------------------------------------------------------------------------
// convenience wrapper to use in parser code
// field << read_args(reader, Base::version) for example
template <typename Reader, typename... Args>
read_argument_helper<Reader, Args...> read_args(Reader& r, Args&&... params)
{
    return read_argument_helper<Reader, Args...>(r, std::forward<Args>(params)...);
}

//------------------------------------------------------------------------------
// now that's the real meat -- field binding
// it does some ugly shit pointer magic inside to get to the fieldset
// while only keeping an unsigned byte inside, using it as an index to infer offset
// to the fieldset pointer somewhere at the bottom of the binding type
template <typename ReadSpec, uint64_t Hash>
struct read_mapped_property : public named_property<Hash>
{
    using value_type = typename ReadSpec::value_type;
    using read_spec = ReadSpec;

    //--------------------------------------------------------------------------
    read_mapped_property(property_initializer&& init) : named_property<Hash>(init.name)
    {
        auto raw_ptr = reinterpret_cast<uint8_t*>(this);
        auto begin_ptr = reinterpret_cast<uint8_t*>(init.host) + sizeof(void*);
        auto diff = raw_ptr - begin_ptr;
        this->index = diff;
    }

    //--------------------------------------------------------------------------
    read_mapped_property<ReadSpec, Hash>& operator=(value_type val)
    {
        this->__get_target()->set(Hash, std::move(val));
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
    template <typename Reader>
    void operator<<(Reader& r)
    {
        *this = ReadSpec::template read<Reader>(r);
    }

    //--------------------------------------------------------------------------
    template <typename Reader, typename... Args>
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
} // namespace hrodvitnir::core

#define MUCH_BLACKER_MAGICK(Prop, ...)                                                                                 \
    read_mapped_property<__VA_ARGS__, #Prop##_crc64> Prop = property_initializer                                       \
    {                                                                                                                  \
        &this->__box, #Prop, Base::ThisClass::register_property(#Prop##_crc64)                                         \
    }
