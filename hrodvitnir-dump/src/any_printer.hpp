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
#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <unordered_map>
#include <hrodvitnir/core/fieldset.hpp>
#include <hrodvitnir/core/fourcc.hpp>
#include <hrodvitnir/core/uuid.hpp>
#include <hrodvitnir/core/tree/tree.hpp>

namespace hrodvitnir::dump
{
    struct dump_context
    {
        dump_context(size_t base_offset_, std::ostream& out_)
            : base_offset(base_offset_)
            , offset(0)
            , out(&out_)
        {

        }

        size_t base_offset;
        size_t offset;
        std::ostream* out;

        template<typename T>
        void print(const T& value)
        {
            *out << value;
        }

        template<typename T>
        dump_context& operator<<(const T& value)
        {
            print(value);
            return *this;
        }
    };

    struct dump_begin_t {};
    struct dump_end_t {};
    struct dump_offset_t {};

    static constexpr const dump_begin_t dump_begin{};
    static constexpr const dump_end_t dump_end{};
    static constexpr const dump_offset_t dump_offset{};

    template<>
    void dump_context::print<dump_begin_t>(const dump_begin_t&)
    {
        offset += base_offset;
    }

    template<>
    void dump_context::print<dump_end_t>(const dump_end_t&)
    {
        offset -= base_offset;
    }

    template<>
    void dump_context::print<dump_offset_t>(const dump_offset_t&)
    {
        for (size_t iter = 0; iter < offset; ++iter) {
            *out << ' ';
        }
    }

    void any_print(dump_context& o, const std::any& v);

    struct any_printer {
        using printer_type = std::function<void(dump_context&, const std::any&)>;

        std::unordered_map<std::type_index, printer_type> _printers;

        template<typename T, typename Fn>
        void register_printer(const Fn& fn)
        {
            auto printer = [fn](dump_context& o, const std::any& v) {
                const T& val = std::any_cast<const T&>(v);
                fn(o, val);
            };

            _printers.emplace(std::type_index(typeid(T)), printer);
        }

        void print(dump_context& o, const std::any& v) const
        {
            auto iter = _printers.find(std::type_index(v.type()));
            if (iter == _printers.end()) {
                o << "<unprintable type " << v.type().name() << ">";
            } else {
                iter->second(o, v);
            }
        }

        any_printer()
        {
            auto simple_printer = [](dump_context& o, const auto& x) { o << x; };
            auto list_printer = [](dump_context& o, const auto& xs) {
                o << "[ ";
                for (const auto& x: xs) {
                    any_print(o, x);
                    o << ", ";
                }
                o << "]";
            };
            auto int8_printer = [](dump_context& o, const auto& x) { o << static_cast<int16_t>(x); };

            register_printer<char>(simple_printer);
            register_printer<uint8_t>(int8_printer);
            register_printer<uint16_t>(simple_printer);
            register_printer<uint32_t>(simple_printer);
            register_printer<uint64_t>(simple_printer);
            register_printer<int8_t>(int8_printer);
            register_printer<int16_t>(simple_printer);
            register_printer<int32_t>(simple_printer);
            register_printer<int64_t>(simple_printer);
            register_printer<float>(simple_printer);
            register_printer<double>(simple_printer);
            register_printer<std::string>(simple_printer);
            register_printer<std::vector<core::fourcc>>(list_printer);
            register_printer<std::vector<double>>(list_printer);
            register_printer<std::vector<uint16_t>>(list_printer);
            register_printer<std::vector<char>>(list_printer);

            register_printer<core::fourcc>([](auto& o, const core::fourcc& fcc) {
                o << fcc.string();
            });

            register_printer<core::uuid>([](auto& o, const core::uuid& u) {
                o << u.to_string();
            });

            register_printer<core::fieldset>([](auto& o, const core::fieldset& fs) {
                o << dump_begin;
                for (const auto& f: fs._fields) {
                    auto name = core::property_base::get_name(f.first);
                    if (name == "__pos" || name == "__size") {
                        continue;
                    }
                    o << dump_offset << "- " << core::property_base::get_name(f.first) << " ";
                    any_print(o, f.second);
                    o << '\n';
                }
                o << dump_end;
            });
        }
    };

    void any_print(dump_context& o, const std::any& v)
    {
        static any_printer printer;
        printer.print(o, v);
    }

    void print_node(dump_context& o, const core::tree_node::ptr& node)
    {
        auto fcc = std::any_cast<core::fourcc>(node->get()->get("type"));
        o << dump_offset << "[box \"" << fcc.string() << "\" " << node->size() << " @ " << node->position() << "]\n";
        any_print(o, *(node->get()));
        if (node->children().size() > 0) {
            o << dump_begin << dump_offset << "- children:\n";
            o << dump_begin;
            for (const auto& child: node->children()) {
                print_node(o, child);
            }
            o << dump_end;
            o << dump_end;
        }
    }

    void print_tree(dump_context& o, const core::tree::ptr& tree)
    {
        for (const auto& node: tree->children()) {
            print_node(o, node);
        }
    }
}
