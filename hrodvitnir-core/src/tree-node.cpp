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

#include <hrodvitnir/core/tree/tree-node.hpp>

namespace hrodvitnir::core
{
    //--------------------------------------------------------------------------
    tree_node::tree_node(std::shared_ptr<fieldset> fs): _value(fs)
    {
        _position = std::any_cast<uint64_t>(fs->get("__pos"_crc64));
        _size = std::any_cast<uint64_t>(fs->get("__size"_crc64));
    }

    //--------------------------------------------------------------------------
    uint64_t tree_node::position() const
    {
        return _position;
    }

    //--------------------------------------------------------------------------
    uint64_t tree_node::size() const
    {
        return _size;
    }

    //--------------------------------------------------------------------------
    uint64_t tree_node::end() const
    {
        return _position + _size;
    }

    //--------------------------------------------------------------------------
    const std::vector<tree_node::ptr>& tree_node::children() const
    {
        return _children;
    }

    //--------------------------------------------------------------------------
    void tree_node::add_child(tree_node::ptr child)
    {
        _children.push_back(child);
    }

    //--------------------------------------------------------------------------
    const std::shared_ptr<fieldset>& tree_node::get() const
    {
        return _value;
    }
}
