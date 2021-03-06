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
#include <cstdint>
#include <memory>
#include <vector>
#include <hrodvitnir/core/fieldset.hpp>

namespace hrodvitnir::core
{
    class tree_node
    {
    public:
        using ptr = std::shared_ptr<tree_node>;

        tree_node(std::shared_ptr<fieldset> fs);
        uint64_t position() const;
        uint64_t size() const;
        uint64_t end() const;
        const std::vector<tree_node::ptr>& children() const;
        void add_child(tree_node::ptr child);
        const std::shared_ptr<fieldset>& get() const;

        tree_node() = delete;
        tree_node(const tree_node&) = delete;
        tree_node& operator=(const tree_node&) = delete;

    private:
        uint64_t _position;
        uint64_t _size;
        std::shared_ptr<fieldset> _value;
        std::vector<tree_node::ptr> _children;
    };
}
