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


namespace hrodvitnir::core
{
    //--------------------------------------------------------------------------
    template<typename Entry>
    class table
    {
    public:
        using entry_type = Entry;

        virtual size_t size() const = 0;
        virtual const entry_type at(size_t index) const = 0;

        const entry_type& operator[](size_t index) const
        {
            return this->at(index);
        }

        virtual ~table() = default;
    };

    //--------------------------------------------------------------------------
    template<typename Entry>
    class array_table: public table<Entry>
    {
    public:
        using entry_type = typename table<Entry>::entry_type;
        using container_type = std::vector<entry_type>;

        //----------------------------------------------------------------------
        array_table() = default;
        array_table(array_table<Entry>&& other) = default;

        //----------------------------------------------------------------------
        size_t size() const override
        {
            return _entries.size();
        }

        //----------------------------------------------------------------------
        const entry_type at(size_t index) const override
        {
            return _entries.at(index);
        }

        //----------------------------------------------------------------------
        const container_type& entries() const
        {
            return _entries;
        }

        //----------------------------------------------------------------------
        container_type& entries()
        {
            return _entries;
        }
    private:
        std::vector<Entry> _entries;
    };

    //--------------------------------------------------------------------------
    template<typename Entry>
    class fixed_table: public table<Entry>
    {
    public:
        using entry_type = typename table<Entry>::entry_type;

        //----------------------------------------------------------------------
        fixed_table(fixed_table<entry_type>&& other)
            : _size(other._size)
            , _entry(std::move(other._entry))
        {

        }

        //----------------------------------------------------------------------
        fixed_table(entry_type entry, size_t size)
            : _entry(std::move(entry))
            , _size(size)
        {

        }

        //----------------------------------------------------------------------
        size_t size() const override
        {
            return _size;
        }

        //----------------------------------------------------------------------
        const entry_type at(size_t index) const override
        {
            if (index >= _size) {
                throw std::range_error("Index out of bounds in fixed table");
            }

            return _entry;
        }

        //----------------------------------------------------------------------
        const entry_type& entry() const
        {
            return _entry;
        }

        //----------------------------------------------------------------------
        entry_type& entry()
        {
            return _entry;
        }

    private:
        size_t _size;
        entry_type _entry;
    };
}