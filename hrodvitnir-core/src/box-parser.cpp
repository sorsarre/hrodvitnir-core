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

#include <hrodvitnir/core/parser/box-parser.hpp>
#include <iostream>

namespace hrodvitnir::core
{
    void box_parser::parse(
            reader_type& reader,
            reader_mapping::ptr mapping,
            box_consumer::ptr consumer
            )
    {
        std::stack<uint64_t> box_ends;

        try {
            while (reader.available() > 0) {
                auto fs = std::make_shared<fieldset>();
                boxes::default_box_t box{*fs};

                box.read_basic(reader);
                // Oops, bad read. Gotta peek or make up read barriers.

                uint64_t current_pos = reader.position() / 8;
                uint64_t current_end = box.box_end();

                if (!box_ends.empty() && current_end > box_ends.top()) {
                    std::cerr << "[box fail] current_end: " << current_end << ", top end: " << box_ends.top() << "\n";
                    // whoa, something has gone wrong
                    std::cerr << "[box fail] current_pos: " << current_pos << "\n";
                    auto bytes_to_skip = box_ends.top() - current_pos;
                    std::cerr << "[box fail] skipping " << bytes_to_skip << " bytes\n";
                    reader.skip(bytes_to_skip*8);

                    while (!box_ends.empty() && box_ends.top() <= current_pos) {
                        auto end = box_ends.top();
                        box_ends.pop();
                        consumer->box_close(end);
                    }
                    continue;
                }

                consumer->box_announce(box.box_pos(), box.uuid);
                const auto& partial = mapping->get(box.uuid);
                partial(*fs, reader);

                current_pos = reader.position() / 8;

                consumer->box_open(fs);
                if (current_pos == current_end) {
                    consumer->box_close(current_end);
                } else {
                    box_ends.push(current_end);
                }

                while (!box_ends.empty() && box_ends.top() <= current_pos) {
                    auto end = box_ends.top();
                    box_ends.pop();
                    consumer->box_close(end);
                }
            }
        } catch (const std::exception& e) {
            while (!box_ends.empty()) {
                consumer->box_close(box_ends.top());
                box_ends.pop();
            }
            // nope
        }
    }
}
