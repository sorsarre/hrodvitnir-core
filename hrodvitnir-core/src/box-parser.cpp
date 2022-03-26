// Copyright (c) 2019-2020, Andrey Kaydalov
// SPDX-FileCopyrightText: 2019-2022 Andrey Kaydalov <sorsarre@gmail.com>
//
// SPDX-License-Identifier: BSD-3-Clause

#include <hrodvitnir/core/parser/box-parser.hpp>
#include <iostream>

namespace hrodvitnir::core
{
void box_parser::parse(reader_type& reader, reader_mapping::ptr mapping, box_consumer::ptr consumer)
{
    std::stack<uint64_t> box_ends;

    try
    {
        while (reader.available() > 0)
        {
            auto fs = std::make_shared<fieldset>();
            boxes::default_box_t box{*fs};

            box.read_basic(reader);
            // Oops, bad read. Gotta peek or make up read barriers.

            uint64_t current_pos = reader.position() / 8;
            uint64_t current_end = box.box_end();

            if (!box_ends.empty() && current_end > box_ends.top())
            {
                std::cerr << "[box fail] current_end: " << current_end << ", top end: " << box_ends.top() << "\n";
                // whoa, something has gone wrong
                std::cerr << "[box fail] current_pos: " << current_pos << "\n";
                auto bytes_to_skip = box_ends.top() - current_pos;
                std::cerr << "[box fail] skipping " << bytes_to_skip << " bytes\n";
                reader.skip(bytes_to_skip * 8);

                while (!box_ends.empty() && box_ends.top() <= current_pos)
                {
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
            if (current_pos == current_end)
            {
                consumer->box_close(current_end);
            }
            else
            {
                box_ends.push(current_end);
            }

            while (!box_ends.empty() && box_ends.top() <= current_pos)
            {
                auto end = box_ends.top();
                box_ends.pop();
                consumer->box_close(end);
            }
        }
    }
    catch (const std::exception& e)
    {
        while (!box_ends.empty())
        {
            consumer->box_close(box_ends.top());
            box_ends.pop();
        }
        // nope
    }
}
} // namespace hrodvitnir::core
