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

#include <hrodvitnir/core/tree/tree-builder.hpp>

namespace hrodvitnir::core
{
    //--------------------------------------------------------------------------
    tree_builder::tree_builder()
    {
        _tree = std::make_shared<tree>();
    }

    //--------------------------------------------------------------------------
    void tree_builder::box_announce(uint64_t position, const uuid& id)
    {
        // nope, nothing to do here
    }

    //--------------------------------------------------------------------------
    void tree_builder::box_open(const std::shared_ptr<fieldset>& fs)
    {
        auto node = std::make_shared<tree_node>(fs);
        if (_stack.empty()) {
            _tree->add_child(node);
        } else {
            _stack.top()->add_child(node);
        }
        _stack.push(node);
    }

    //--------------------------------------------------------------------------
    void tree_builder::box_close(uint64_t position)
    {
        _stack.pop();
    }

    //--------------------------------------------------------------------------
    tree::ptr tree_builder::get() const
    {
        return _tree; // TODO: Make it an honest const
    }
}
